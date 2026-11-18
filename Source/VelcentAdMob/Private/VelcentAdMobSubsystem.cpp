#include "VelcentAdMobSubsystem.h"

#include "Async/Async.h"
#include "VelcentAdMob.h"
#include "VelcentAdMobSettings.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

namespace
{
	FString VelcentAdMobFStringFromJString(JNIEnv* Env, jstring Value)
	{
		if (!Env || !Value)
		{
			return FString();
		}

		const char* Chars = Env->GetStringUTFChars(Value, nullptr);
		FString Result = Chars ? UTF8_TO_TCHAR(Chars) : FString();
		if (Chars)
		{
			Env->ReleaseStringUTFChars(Value, Chars);
		}
		return Result;
	}
}
#endif

namespace
{
	TWeakObjectPtr<UVelcentAdMobSubsystem> GVelcentAdMobSubsystem;
}

namespace VelcentAdMobBridge
{
	void EmitAdEvent(EVelcentAdMobAdType AdType, EVelcentAdMobAdEvent Event, const FString& AdUnitId, int32 ErrorCode, const FString& Message)
	{
		AsyncTask(ENamedThreads::GameThread, [AdType, Event, AdUnitId, ErrorCode, Message]()
		{
			if (GVelcentAdMobSubsystem.IsValid())
			{
				GVelcentAdMobSubsystem->HandleNativeAdEvent(AdType, Event, AdUnitId, ErrorCode, Message);
			}
		});
	}

	void EmitReward(const FString& AdUnitId, const FString& RewardType, int32 Amount)
	{
		AsyncTask(ENamedThreads::GameThread, [AdUnitId, RewardType, Amount]()
		{
			if (GVelcentAdMobSubsystem.IsValid())
			{
				GVelcentAdMobSubsystem->HandleNativeReward(AdUnitId, RewardType, Amount);
			}
		});
	}
}

namespace VelcentAdMobNative
{
#if PLATFORM_ANDROID
	bool CallVoidMethod(const ANSICHAR* MethodName, const ANSICHAR* Signature)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		if (!Env)
		{
			return false;
		}

		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, MethodName, Signature, false);
		if (!Method)
		{
			return false;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
		return true;
	}

	bool CallVoidStringMethod(const ANSICHAR* MethodName, const ANSICHAR* Signature, const FString& Value)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		if (!Env)
		{
			return false;
		}

		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, MethodName, Signature, false);
		if (!Method)
		{
			return false;
		}

		auto JavaString = FJavaHelper::ToJavaString(Env, Value);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, *JavaString);
		return true;
	}

	bool CallBooleanMethod(const ANSICHAR* MethodName, const ANSICHAR* Signature)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		if (!Env)
		{
			return false;
		}

		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, MethodName, Signature, false);
		return Method ? FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method) : false;
	}

	bool Initialize()
	{
		return CallVoidMethod("AndroidThunkJava_VelcentAdMobInitialize", "()V");
	}

	bool ShowBanner(const FString& AdUnitId, EVelcentAdMobBannerPosition Position, bool bAdaptive)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		if (!Env)
		{
			return false;
		}

		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_VelcentAdMobShowBanner", "(Ljava/lang/String;IZ)V", false);
		if (!Method)
		{
			return false;
		}

		auto JavaAdUnitId = FJavaHelper::ToJavaString(Env, AdUnitId);
		const jboolean bJavaAdaptive = bAdaptive ? JNI_TRUE : JNI_FALSE;
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, *JavaAdUnitId, static_cast<int32>(Position), bJavaAdaptive);
		return true;
	}

	bool HideBanner()
	{
		return CallVoidMethod("AndroidThunkJava_VelcentAdMobHideBanner", "()V");
	}

	bool DestroyBanner()
	{
		return CallVoidMethod("AndroidThunkJava_VelcentAdMobDestroyBanner", "()V");
	}

	bool LoadInterstitial(const FString& AdUnitId)
	{
		return CallVoidStringMethod("AndroidThunkJava_VelcentAdMobLoadInterstitial", "(Ljava/lang/String;)V", AdUnitId);
	}

	bool IsInterstitialReady()
	{
		return CallBooleanMethod("AndroidThunkJava_VelcentAdMobIsInterstitialReady", "()Z");
	}

	bool ShowInterstitial()
	{
		return CallVoidMethod("AndroidThunkJava_VelcentAdMobShowInterstitial", "()V");
	}

	bool LoadRewarded(const FString& AdUnitId)
	{
		return CallVoidStringMethod("AndroidThunkJava_VelcentAdMobLoadRewarded", "(Ljava/lang/String;)V", AdUnitId);
	}

	bool IsRewardedReady()
	{
		return CallBooleanMethod("AndroidThunkJava_VelcentAdMobIsRewardedReady", "()Z");
	}

	bool ShowRewarded()
	{
		return CallVoidMethod("AndroidThunkJava_VelcentAdMobShowRewarded", "()V");
	}
#elif PLATFORM_IOS
	bool Initialize();
	bool ShowBanner(const FString& AdUnitId, EVelcentAdMobBannerPosition Position, bool bAdaptive);
	bool HideBanner();
	bool DestroyBanner();
	bool LoadInterstitial(const FString& AdUnitId);
	bool IsInterstitialReady();
	bool ShowInterstitial();
	bool LoadRewarded(const FString& AdUnitId);
	bool IsRewardedReady();
	bool ShowRewarded();
#else
	bool Initialize() { return false; }
	bool ShowBanner(const FString&, EVelcentAdMobBannerPosition, bool) { return false; }
	bool HideBanner() { return false; }
	bool DestroyBanner() { return false; }
	bool LoadInterstitial(const FString&) { return false; }
	bool IsInterstitialReady() { return false; }
	bool ShowInterstitial() { return false; }
	bool LoadRewarded(const FString&) { return false; }
	bool IsRewardedReady() { return false; }
	bool ShowRewarded() { return false; }
#endif
}

#if PLATFORM_ANDROID
extern "C"
{
	JNI_METHOD void Java_com_epicgames_unreal_GameActivity_nativeVelcentAdMobOnAdEvent(JNIEnv* Env, jclass, jstring AdUnitId, jint AdType, jint EventType, jint ErrorCode, jstring Message)
	{
		const FString ConvertedAdUnitId = VelcentAdMobFStringFromJString(Env, AdUnitId);
		const FString ConvertedMessage = VelcentAdMobFStringFromJString(Env, Message);
		VelcentAdMobBridge::EmitAdEvent(
			static_cast<EVelcentAdMobAdType>(static_cast<uint8>(AdType)),
			static_cast<EVelcentAdMobAdEvent>(static_cast<uint8>(EventType)),
			ConvertedAdUnitId,
			static_cast<int32>(ErrorCode),
			ConvertedMessage);
	}

	JNI_METHOD void Java_com_epicgames_unreal_GameActivity_nativeVelcentAdMobOnReward(JNIEnv* Env, jclass, jstring AdUnitId, jstring RewardType, jint Amount)
	{
		const FString ConvertedAdUnitId = VelcentAdMobFStringFromJString(Env, AdUnitId);
		const FString ConvertedRewardType = VelcentAdMobFStringFromJString(Env, RewardType);
		VelcentAdMobBridge::EmitReward(ConvertedAdUnitId, ConvertedRewardType, static_cast<int32>(Amount));
	}
}
#endif

void UVelcentAdMobSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GVelcentAdMobSubsystem = this;

	if (const UVelcentAdMobSettings* Settings = GetSettings())
	{
		if (Settings->bInitializeOnGameInstanceStart)
		{
			InitializeAdMob();
		}
	}
}

void UVelcentAdMobSubsystem::Deinitialize()
{
	if (GVelcentAdMobSubsystem.Get() == this)
	{
		GVelcentAdMobSubsystem.Reset();
	}

	Super::Deinitialize();
}

void UVelcentAdMobSubsystem::InitializeAdMob()
{
	FString Reason;
	if (!CanUseAds(&Reason))
	{
		BroadcastEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::Unavailable, FString(), 0, Reason);
		return;
	}

	if (!VelcentAdMobNative::Initialize())
	{
		BroadcastEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::InitializationFailed, FString(), 0, TEXT("Native AdMob bridge is unavailable."));
	}
}

void UVelcentAdMobSubsystem::ShowDefaultBanner(EVelcentAdMobBannerPosition Position)
{
	const UVelcentAdMobSettings* Settings = GetSettings();
	ShowBanner(Settings ? Settings->GetDefaultBannerAdUnitId() : FString(), Position, Settings ? Settings->bUseAdaptiveBanner : true);
}

void UVelcentAdMobSubsystem::ShowBanner(const FString& AdUnitId, EVelcentAdMobBannerPosition Position, bool bAdaptive)
{
	if (AdUnitId.IsEmpty())
	{
		BroadcastEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::FailedToLoad, AdUnitId, 0, TEXT("Banner ad unit id is empty."));
		return;
	}

	if (!VelcentAdMobNative::ShowBanner(AdUnitId, Position, bAdaptive))
	{
		BroadcastEvent(EVelcentAdMobAdType::Banner, EVelcentAdMobAdEvent::Unavailable, AdUnitId, 0, TEXT("Native banner bridge is unavailable."));
	}
}

void UVelcentAdMobSubsystem::HideBanner()
{
	VelcentAdMobNative::HideBanner();
}

void UVelcentAdMobSubsystem::DestroyBanner()
{
	VelcentAdMobNative::DestroyBanner();
}

void UVelcentAdMobSubsystem::LoadDefaultInterstitial()
{
	const UVelcentAdMobSettings* Settings = GetSettings();
	LoadInterstitial(Settings ? Settings->GetDefaultInterstitialAdUnitId() : FString());
}

void UVelcentAdMobSubsystem::LoadInterstitial(const FString& AdUnitId)
{
	if (AdUnitId.IsEmpty())
	{
		BroadcastEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::FailedToLoad, AdUnitId, 0, TEXT("Interstitial ad unit id is empty."));
		return;
	}

	LastInterstitialAdUnitId = AdUnitId;
	bInterstitialReady = false;
	if (!VelcentAdMobNative::LoadInterstitial(AdUnitId))
	{
		BroadcastEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::Unavailable, AdUnitId, 0, TEXT("Native interstitial bridge is unavailable."));
	}
}

void UVelcentAdMobSubsystem::ShowInterstitial()
{
	if (!VelcentAdMobNative::ShowInterstitial())
	{
		BroadcastEvent(EVelcentAdMobAdType::Interstitial, EVelcentAdMobAdEvent::Unavailable, LastInterstitialAdUnitId, 0, TEXT("Native interstitial bridge is unavailable."));
	}
}

void UVelcentAdMobSubsystem::LoadDefaultRewarded()
{
	const UVelcentAdMobSettings* Settings = GetSettings();
	LoadRewarded(Settings ? Settings->GetDefaultRewardedAdUnitId() : FString());
}

void UVelcentAdMobSubsystem::LoadRewarded(const FString& AdUnitId)
{
	if (AdUnitId.IsEmpty())
	{
		BroadcastEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::FailedToLoad, AdUnitId, 0, TEXT("Rewarded ad unit id is empty."));
		return;
	}

	LastRewardedAdUnitId = AdUnitId;
	bRewardedReady = false;
	if (!VelcentAdMobNative::LoadRewarded(AdUnitId))
	{
		BroadcastEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::Unavailable, AdUnitId, 0, TEXT("Native rewarded bridge is unavailable."));
	}
}

void UVelcentAdMobSubsystem::ShowRewarded()
{
	if (!VelcentAdMobNative::ShowRewarded())
	{
		BroadcastEvent(EVelcentAdMobAdType::Rewarded, EVelcentAdMobAdEvent::Unavailable, LastRewardedAdUnitId, 0, TEXT("Native rewarded bridge is unavailable."));
	}
}

EVelcentAdMobPlatform UVelcentAdMobSubsystem::GetRuntimePlatform() const
{
#if PLATFORM_ANDROID
	return EVelcentAdMobPlatform::Android;
#elif PLATFORM_IOS
	return EVelcentAdMobPlatform::IOS;
#elif WITH_EDITOR
	return EVelcentAdMobPlatform::Editor;
#else
	return EVelcentAdMobPlatform::Unsupported;
#endif
}

void UVelcentAdMobSubsystem::HandleNativeAdEvent(EVelcentAdMobAdType AdType, EVelcentAdMobAdEvent Event, const FString& AdUnitId, int32 ErrorCode, const FString& Message)
{
	switch (Event)
	{
	case EVelcentAdMobAdEvent::Initialized:
		bInitialized = true;
		break;
	case EVelcentAdMobAdEvent::Loaded:
		if (AdType == EVelcentAdMobAdType::Interstitial)
		{
			bInterstitialReady = true;
		}
		else if (AdType == EVelcentAdMobAdType::Rewarded)
		{
			bRewardedReady = true;
		}
		break;
	case EVelcentAdMobAdEvent::FailedToLoad:
	case EVelcentAdMobAdEvent::FailedToShow:
	case EVelcentAdMobAdEvent::Closed:
		if (AdType == EVelcentAdMobAdType::Interstitial)
		{
			bInterstitialReady = false;
		}
		else if (AdType == EVelcentAdMobAdType::Rewarded)
		{
			bRewardedReady = false;
		}
		break;
	default:
		break;
	}

	BroadcastEvent(AdType, Event, AdUnitId, ErrorCode, Message);
}

void UVelcentAdMobSubsystem::HandleNativeReward(const FString& AdUnitId, const FString& RewardType, int32 Amount)
{
	BroadcastReward(AdUnitId, RewardType, Amount);
}

const UVelcentAdMobSettings* UVelcentAdMobSubsystem::GetSettings() const
{
	return GetDefault<UVelcentAdMobSettings>();
}

bool UVelcentAdMobSubsystem::CanUseAds(FString* OutReason) const
{
	const UVelcentAdMobSettings* Settings = GetSettings();
	if (!Settings || !Settings->bEnableAdMob)
	{
		if (OutReason)
		{
			*OutReason = TEXT("AdMob is disabled in Velcent AdMob settings.");
		}
		return false;
	}

	if (GetRuntimePlatform() == EVelcentAdMobPlatform::Unsupported || GetRuntimePlatform() == EVelcentAdMobPlatform::Editor)
	{
		if (OutReason)
		{
			*OutReason = TEXT("AdMob is only available on Android and iOS runtime builds.");
		}
		return false;
	}

	return true;
}

void UVelcentAdMobSubsystem::BroadcastEvent(EVelcentAdMobAdType AdType, EVelcentAdMobAdEvent Event, const FString& AdUnitId, int32 ErrorCode, const FString& Message)
{
	FVelcentAdMobEventPayload Payload;
	Payload.AdType = AdType;
	Payload.Event = Event;
	Payload.AdUnitId = AdUnitId;
	Payload.ErrorCode = ErrorCode;
	Payload.Message = Message;

	UE_LOG(LogVelcentAdMob, Verbose, TEXT("Ad event: Type=%d Event=%d Unit=%s Error=%d Message=%s"),
		static_cast<int32>(AdType),
		static_cast<int32>(Event),
		*AdUnitId,
		ErrorCode,
		*Message);

	OnAdEvent.Broadcast(Payload);
}

void UVelcentAdMobSubsystem::BroadcastReward(const FString& AdUnitId, const FString& RewardType, int32 Amount)
{
	FVelcentAdMobEventPayload Payload;
	Payload.AdType = EVelcentAdMobAdType::Rewarded;
	Payload.Event = EVelcentAdMobAdEvent::RewardEarned;
	Payload.AdUnitId = AdUnitId;
	Payload.Reward.Type = RewardType;
	Payload.Reward.Amount = Amount;

	OnAdEvent.Broadcast(Payload);
}
