#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "VelcentAdMobSettings.generated.h"

UCLASS(Config = Engine, DefaultConfig, meta = (DisplayName = "Velcent AdMob"))
class VELCENTADMOB_API UVelcentAdMobSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	bool bEnableAdMob = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	bool bUseTestAdUnitIds = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	bool bInitializeOnGameInstanceStart = false;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android")
	FString AndroidAdMobApplicationId = TEXT("ca-app-pub-3940256099942544~3347511713");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS")
	FString IOSAdMobApplicationId = TEXT("ca-app-pub-3940256099942544~1458002511");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android|Test Units")
	FString AndroidTestBannerAdUnitId = TEXT("ca-app-pub-3940256099942544/6300978111");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android|Test Units")
	FString AndroidTestInterstitialAdUnitId = TEXT("ca-app-pub-3940256099942544/1033173712");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android|Test Units")
	FString AndroidTestRewardedAdUnitId = TEXT("ca-app-pub-3940256099942544/5224354917");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS|Test Units")
	FString IOSTestBannerAdUnitId = TEXT("ca-app-pub-3940256099942544/2934735716");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS|Test Units")
	FString IOSTestInterstitialAdUnitId = TEXT("ca-app-pub-3940256099942544/4411468910");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS|Test Units")
	FString IOSTestRewardedAdUnitId = TEXT("ca-app-pub-3940256099942544/1712485313");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android|Production Units")
	FString AndroidBannerAdUnitId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android|Production Units")
	FString AndroidInterstitialAdUnitId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Android|Production Units")
	FString AndroidRewardedAdUnitId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS|Production Units")
	FString IOSBannerAdUnitId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS|Production Units")
	FString IOSInterstitialAdUnitId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "iOS|Production Units")
	FString IOSRewardedAdUnitId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Request")
	bool bUseAdaptiveBanner = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Request")
	bool bTagForChildDirectedTreatment = false;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Request")
	bool bTagForUnderAgeOfConsent = false;

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob|Settings")
	FString GetDefaultBannerAdUnitId() const;

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob|Settings")
	FString GetDefaultInterstitialAdUnitId() const;

	UFUNCTION(BlueprintPure, Category = "Velcent|AdMob|Settings")
	FString GetDefaultRewardedAdUnitId() const;
};
