#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VelcentAdMobTypes.h"
#include "VelcentAdMobSubsystem.generated.h"

class UVelcentAdMobSettings;

namespace VelcentAdMobBridge
{
	VELCENTADMOB_API void EmitAdEvent(EVelcentAdMobAdType AdType, EVelcentAdMobAdEvent Event, const FString& AdUnitId, int32 ErrorCode, const FString& Message);
	VELCENTADMOB_API void EmitReward(const FString& AdUnitId, const FString& RewardType, int32 Amount);
}

UCLASS()
class VELCENTADMOB_API UVelcentAdMobSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob")
	void InitializeAdMob();

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner")
	void ShowDefaultBanner(EVelcentAdMobBannerPosition Position);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner")
	void ShowBanner(const FString& AdUnitId, EVelcentAdMobBannerPosition Position, bool bAdaptive);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner")
	void HideBanner();

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner")
	void DestroyBanner();

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Interstitial")
	void LoadDefaultInterstitial();

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Interstitial")
	void LoadInterstitial(const FString& AdUnitId);

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob|Interstitial")
	bool IsInterstitialReady() const { return bInterstitialReady; }

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Interstitial")
	void ShowInterstitial();

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Rewarded")
	void LoadDefaultRewarded();

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Rewarded")
	void LoadRewarded(const FString& AdUnitId);

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob|Rewarded")
	bool IsRewardedReady() const { return bRewardedReady; }

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Rewarded")
	void ShowRewarded();

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob")
	bool IsInitialized() const { return bInitialized; }

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob")
	EVelcentAdMobPlatform GetRuntimePlatform() const;

	void HandleNativeAdEvent(EVelcentAdMobAdType AdType, EVelcentAdMobAdEvent Event, const FString& AdUnitId, int32 ErrorCode, const FString& Message);
	void HandleNativeReward(const FString& AdUnitId, const FString& RewardType, int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Velcent|AdMob")
	FVelcentAdMobEventSignature OnAdEvent;

protected:
	const UVelcentAdMobSettings* GetSettings() const;
	bool CanUseAds(FString* OutReason = nullptr) const;
	void BroadcastEvent(EVelcentAdMobAdType AdType, EVelcentAdMobAdEvent Event, const FString& AdUnitId, int32 ErrorCode = 0, const FString& Message = FString());
	void BroadcastReward(const FString& AdUnitId, const FString& RewardType, int32 Amount);

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	bool bInitialized = false;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	bool bInterstitialReady = false;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	bool bRewardedReady = false;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	FString LastInterstitialAdUnitId;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	FString LastRewardedAdUnitId;
};
