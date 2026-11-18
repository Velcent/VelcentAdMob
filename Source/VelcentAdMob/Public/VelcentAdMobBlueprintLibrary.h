#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VelcentAdMobTypes.h"
#include "VelcentAdMobBlueprintLibrary.generated.h"

class UVelcentAdMobSubsystem;

UCLASS()
class VELCENTADMOB_API UVelcentAdMobBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob", meta = (WorldContext = "WorldContextObject"))
	static UVelcentAdMobSubsystem* GetVelcentAdMobSubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob", meta = (WorldContext = "WorldContextObject"))
	static void InitializeAdMob(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner", meta = (WorldContext = "WorldContextObject"))
	static void ShowDefaultBanner(const UObject* WorldContextObject, EVelcentAdMobBannerPosition Position);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner", meta = (WorldContext = "WorldContextObject"))
	static void HideBanner(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Banner", meta = (WorldContext = "WorldContextObject"))
	static void DestroyBanner(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Interstitial", meta = (WorldContext = "WorldContextObject"))
	static void LoadDefaultInterstitial(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Interstitial", meta = (WorldContext = "WorldContextObject"))
	static void ShowInterstitial(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Rewarded", meta = (WorldContext = "WorldContextObject"))
	static void LoadDefaultRewarded(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Velcent|AdMob|Rewarded", meta = (WorldContext = "WorldContextObject"))
	static void ShowRewarded(const UObject* WorldContextObject);
};
