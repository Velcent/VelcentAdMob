#include "VelcentAdMobSettings.h"

namespace
{
	bool IsAndroidRuntime()
	{
#if PLATFORM_ANDROID
		return true;
#else
		return false;
#endif
	}

	bool IsIOSRuntime()
	{
#if PLATFORM_IOS
		return true;
#else
		return false;
#endif
	}
}

FString UVelcentAdMobSettings::GetDefaultBannerAdUnitId() const
{
	if (IsAndroidRuntime())
	{
		return bUseTestAdUnitIds || AndroidBannerAdUnitId.IsEmpty() ? AndroidTestBannerAdUnitId : AndroidBannerAdUnitId;
	}

	if (IsIOSRuntime())
	{
		return bUseTestAdUnitIds || IOSBannerAdUnitId.IsEmpty() ? IOSTestBannerAdUnitId : IOSBannerAdUnitId;
	}

	return FString();
}

FString UVelcentAdMobSettings::GetDefaultInterstitialAdUnitId() const
{
	if (IsAndroidRuntime())
	{
		return bUseTestAdUnitIds || AndroidInterstitialAdUnitId.IsEmpty() ? AndroidTestInterstitialAdUnitId : AndroidInterstitialAdUnitId;
	}

	if (IsIOSRuntime())
	{
		return bUseTestAdUnitIds || IOSInterstitialAdUnitId.IsEmpty() ? IOSTestInterstitialAdUnitId : IOSInterstitialAdUnitId;
	}

	return FString();
}

FString UVelcentAdMobSettings::GetDefaultRewardedAdUnitId() const
{
	if (IsAndroidRuntime())
	{
		return bUseTestAdUnitIds || AndroidRewardedAdUnitId.IsEmpty() ? AndroidTestRewardedAdUnitId : AndroidRewardedAdUnitId;
	}

	if (IsIOSRuntime())
	{
		return bUseTestAdUnitIds || IOSRewardedAdUnitId.IsEmpty() ? IOSTestRewardedAdUnitId : IOSRewardedAdUnitId;
	}

	return FString();
}
