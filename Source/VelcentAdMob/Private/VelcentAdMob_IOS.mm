#include "HAL/Platform.h"

#if PLATFORM_IOS

#include "VelcentAdMobSubsystem.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#if __has_include(<GoogleMobileAds/GoogleMobileAds.h>)
#import <GoogleMobileAds/GoogleMobileAds.h>
#define VELCENTADMOB_WITH_IOS_GMA 1
#else
#define VELCENTADMOB_WITH_IOS_GMA 0
#endif

namespace
{
	NSString* VelcentAdMobToNSString(const FString& Value)
	{
		return [NSString stringWithUTF8String:TCHAR_TO_UTF8(*Value)];
	}

	FString VelcentAdMobToFString(NSString* Value)
	{
		return Value ? FString(UTF8_TO_TCHAR([Value UTF8String])) : FString();
	}
}

#if VELCENTADMOB_WITH_IOS_GMA

@interface VelcentAdMobIOSBridge : NSObject<GADBannerViewDelegate, GADFullScreenContentDelegate>
@property(nonatomic, strong) GADBannerView* BannerView;
@property(nonatomic, strong) GADInterstitialAd* InterstitialAd;
@property(nonatomic, strong) GADRewardedAd* RewardedAd;
@property(nonatomic, copy) NSString* BannerAdUnitId;
@property(nonatomic, copy) NSString* InterstitialAdUnitId;
@property(nonatomic, copy) NSString* RewardedAdUnitId;
@end

@implementation VelcentAdMobIOSBridge

- (UIViewController*)rootViewController
{
	UIWindow* KeyWindow = nil;
	for (UIWindow* Window in [UIApplication sharedApplication].windows)
	{
		if (Window.isKeyWindow)
		{
			KeyWindow = Window;
			break;
		}
	}

	UIViewController* Root = KeyWindow.rootViewController;
	while (Root.presentedViewController)
	{
		Root = Root.presentedViewController;
	}
	return Root;
}

- (void)initialize
{
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::InitializeStarted, FString(), 0, FString());
	[[GADMobileAds sharedInstance] startWithCompletionHandler:^(GADInitializationStatus* Status)
	{
		VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::Initialized, FString(), 0, FString());
	}];
}

- (void)showBanner:(NSString*)AdUnitId position:(EVelcentAdMobBannerPosition)Position adaptive:(bool)bAdaptive
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[self destroyBanner];

		UIViewController* Root = [self rootViewController];
		if (!Root)
		{
			VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::FailedToLoad, VelcentAdMobToFString(AdUnitId), 0, TEXT("No iOS root view controller."));
			return;
		}

		CGFloat Width = Root.view.bounds.size.width;
		GADAdSize Size = bAdaptive ? GADCurrentOrientationAnchoredAdaptiveBannerAdSizeWithWidth(Width) : GADAdSizeBanner;
		self.BannerView = [[GADBannerView alloc] initWithAdSize:Size];
		self.BannerAdUnitId = AdUnitId;
		self.BannerView.adUnitID = AdUnitId;
		self.BannerView.rootViewController = Root;
		self.BannerView.delegate = self;

		CGSize BannerSize = self.BannerView.bounds.size;
		CGFloat Y = Position == EVelcentAdMobBannerPosition::Bottom
			? Root.view.bounds.size.height - BannerSize.height
			: 0.0f;
		self.BannerView.frame = CGRectMake((Root.view.bounds.size.width - BannerSize.width) * 0.5f, Y, BannerSize.width, BannerSize.height);
		self.BannerView.autoresizingMask = Position == EVelcentAdMobBannerPosition::Bottom
			? UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin
			: UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin;
		[Root.view addSubview:self.BannerView];

		VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::LoadStarted, VelcentAdMobToFString(AdUnitId), 0, FString());
		[self.BannerView loadRequest:[GADRequest request]];
	});
}

- (void)hideBanner
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		self.BannerView.hidden = YES;
	});
}

- (void)destroyBanner
{
	if (self.BannerView)
	{
		[self.BannerView removeFromSuperview];
		self.BannerView.delegate = nil;
		self.BannerView = nil;
	}
}

- (void)loadInterstitial:(NSString*)AdUnitId
{
	self.InterstitialAdUnitId = AdUnitId;
	self.InterstitialAd = nil;
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::LoadStarted, VelcentAdMobToFString(AdUnitId), 0, FString());
	[GADInterstitialAd loadWithAdUnitID:AdUnitId request:[GADRequest request] completionHandler:^(GADInterstitialAd* Ad, NSError* Error)
	{
		if (Error)
		{
			VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::FailedToLoad, VelcentAdMobToFString(AdUnitId), (int32)Error.code, VelcentAdMobToFString(Error.localizedDescription));
			return;
		}

		self.InterstitialAd = Ad;
		self.InterstitialAd.fullScreenContentDelegate = self;
		VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::Loaded, VelcentAdMobToFString(AdUnitId), 0, FString());
	}];
}

- (void)showInterstitial
{
	UIViewController* Root = [self rootViewController];
	if (!Root || !self.InterstitialAd)
	{
		VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::FailedToShow, VelcentAdMobToFString(self.InterstitialAdUnitId), 0, TEXT("Interstitial is not ready."));
		return;
	}

	[self.InterstitialAd presentFromRootViewController:Root];
}

- (void)loadRewarded:(NSString*)AdUnitId
{
	self.RewardedAdUnitId = AdUnitId;
	self.RewardedAd = nil;
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::LoadStarted, VelcentAdMobToFString(AdUnitId), 0, FString());
	[GADRewardedAd loadWithAdUnitID:AdUnitId request:[GADRequest request] completionHandler:^(GADRewardedAd* Ad, NSError* Error)
	{
		if (Error)
		{
			VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::FailedToLoad, VelcentAdMobToFString(AdUnitId), (int32)Error.code, VelcentAdMobToFString(Error.localizedDescription));
			return;
		}

		self.RewardedAd = Ad;
		self.RewardedAd.fullScreenContentDelegate = self;
		VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::Loaded, VelcentAdMobToFString(AdUnitId), 0, FString());
	}];
}

- (void)showRewarded
{
	UIViewController* Root = [self rootViewController];
	if (!Root || !self.RewardedAd)
	{
		VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::FailedToShow, VelcentAdMobToFString(self.RewardedAdUnitId), 0, TEXT("Rewarded ad is not ready."));
		return;
	}

	GADRewardedAd* AdToShow = self.RewardedAd;
	[AdToShow presentFromRootViewController:Root userDidEarnRewardHandler:^
	{
		GADAdReward* Reward = AdToShow.adReward;
		VelcentAdMobBridge::EmitReward(VelcentAdMobToFString(self.RewardedAdUnitId), VelcentAdMobToFString(Reward.type), Reward.amount.intValue);
	}];
}

- (void)bannerViewDidReceiveAd:(GADBannerView*)BannerView
{
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::Loaded, VelcentAdMobToFString(self.BannerAdUnitId), 0, FString());
}

- (void)bannerView:(GADBannerView*)BannerView didFailToReceiveAdWithError:(NSError*)Error
{
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::FailedToLoad, VelcentAdMobToFString(self.BannerAdUnitId), (int32)Error.code, VelcentAdMobToFString(Error.localizedDescription));
}

- (void)bannerViewDidRecordClick:(GADBannerView*)BannerView
{
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::Clicked, VelcentAdMobToFString(self.BannerAdUnitId), 0, FString());
}

- (void)bannerViewDidRecordImpression:(GADBannerView*)BannerView
{
	VelcentAdMobBridge::EmitAdEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::Impression, VelcentAdMobToFString(self.BannerAdUnitId), 0, FString());
}

- (void)adWillPresentFullScreenContent:(id<GADFullScreenPresentingAd>)Ad
{
	EVelcentAdMobAdType Type = Ad == self.InterstitialAd ? EVelcentAdMobAdType::Interstitial : EVelcentAdMobAdType::Rewarded;
	NSString* AdUnitId = Type == EVelcentAdMobAdType::Interstitial ? self.InterstitialAdUnitId : self.RewardedAdUnitId;
	VelcentAdMobBridge::EmitAdEvent(Type, EVelcentAdMobAdEvent::Opened, VelcentAdMobToFString(AdUnitId), 0, FString());
}

- (void)adDidDismissFullScreenContent:(id<GADFullScreenPresentingAd>)Ad
{
	EVelcentAdMobAdType Type = Ad == self.InterstitialAd ? EVelcentAdMobAdType::Interstitial : EVelcentAdMobAdType::Rewarded;
	NSString* AdUnitId = Type == EVelcentAdMobAdType::Interstitial ? self.InterstitialAdUnitId : self.RewardedAdUnitId;
	if (Type == EVelcentAdMobAdType::Interstitial)
	{
		self.InterstitialAd = nil;
	}
	else
	{
		self.RewardedAd = nil;
	}
	VelcentAdMobBridge::EmitAdEvent(Type, EVelcentAdMobAdEvent::Closed, VelcentAdMobToFString(AdUnitId), 0, FString());
}

- (void)ad:(id<GADFullScreenPresentingAd>)Ad didFailToPresentFullScreenContentWithError:(NSError*)Error
{
	EVelcentAdMobAdType Type = Ad == self.InterstitialAd ? EVelcentAdMobAdType::Interstitial : EVelcentAdMobAdType::Rewarded;
	NSString* AdUnitId = Type == EVelcentAdMobAdType::Interstitial ? self.InterstitialAdUnitId : self.RewardedAdUnitId;
	VelcentAdMobBridge::EmitAdEvent(Type, EVelcentAdMobAdEvent::FailedToShow, VelcentAdMobToFString(AdUnitId), (int32)Error.code, VelcentAdMobToFString(Error.localizedDescription));
}

@end

static VelcentAdMobIOSBridge* VelcentAdMobGetIOSBridge()
{
	static VelcentAdMobIOSBridge* Bridge = [[VelcentAdMobIOSBridge alloc] init];
	return Bridge;
}

#endif

namespace VelcentAdMobNative
{
	bool Initialize()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() initialize];
		return true;
#else
		return false;
#endif
	}

	bool ShowBanner(const FString& AdUnitId, EVelcentAdMobBannerPosition Position, bool bAdaptive)
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() showBanner:VelcentAdMobToNSString(AdUnitId) position:Position adaptive:bAdaptive];
		return true;
#else
		return false;
#endif
	}

	bool HideBanner()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() hideBanner];
		return true;
#else
		return false;
#endif
	}

	bool DestroyBanner()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() destroyBanner];
		return true;
#else
		return false;
#endif
	}

	bool LoadInterstitial(const FString& AdUnitId)
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() loadInterstitial:VelcentAdMobToNSString(AdUnitId)];
		return true;
#else
		return false;
#endif
	}

	bool IsInterstitialReady()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		return [VelcentAdMobGetIOSBridge() InterstitialAd] != nil;
#else
		return false;
#endif
	}

	bool ShowInterstitial()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() showInterstitial];
		return true;
#else
		return false;
#endif
	}

	bool LoadRewarded(const FString& AdUnitId)
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() loadRewarded:VelcentAdMobToNSString(AdUnitId)];
		return true;
#else
		return false;
#endif
	}

	bool IsRewardedReady()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		return [VelcentAdMobGetIOSBridge() RewardedAd] != nil;
#else
		return false;
#endif
	}

	bool ShowRewarded()
	{
#if VELCENTADMOB_WITH_IOS_GMA
		[VelcentAdMobGetIOSBridge() showRewarded];
		return true;
#else
		return false;
#endif
	}
}

#endif
