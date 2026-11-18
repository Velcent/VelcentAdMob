#pragma once

#include "CoreMinimal.h"
#include "VelcentAdMobTypes.generated.h"

UENUM(BlueprintType)
enum class EVelcentAdMobAdType : uint8
{
	Banner,
	Interstitial,
	Rewarded,
	AppOpen
};

UENUM(BlueprintType)
enum class EVelcentAdMobAdEvent : uint8
{
	InitializeStarted,
	Initialized,
	InitializationFailed,
	LoadStarted,
	Loaded,
	FailedToLoad,
	Opened,
	Closed,
	FailedToShow,
	Clicked,
	Impression,
	RewardEarned,
	Unavailable
};

UENUM(BlueprintType)
enum class EVelcentAdMobBannerPosition : uint8
{
	Top,
	Bottom
};

UENUM(BlueprintType)
enum class EVelcentAdMobPlatform : uint8
{
	Unsupported,
	Android,
	IOS,
	Editor
};

USTRUCT(BlueprintType)
struct FVelcentAdMobRewardItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	FString Type;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	int32 Amount = 0;
};

USTRUCT(BlueprintType)
struct FVelcentAdMobEventPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	EVelcentAdMobAdType AdType = EVelcentAdMobAdType::Banner;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	EVelcentAdMobAdEvent Event = EVelcentAdMobAdEvent::Unavailable;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	FString AdUnitId;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	int32 ErrorCode = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	FString Message;

	UPROPERTY(BlueprintReadOnly, Category = "Velcent|AdMob")
	FVelcentAdMobRewardItem Reward;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVelcentAdMobEventSignature, const FVelcentAdMobEventPayload&, Payload);
