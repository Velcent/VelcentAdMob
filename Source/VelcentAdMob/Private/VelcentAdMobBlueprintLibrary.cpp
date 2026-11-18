#include "VelcentAdMobBlueprintLibrary.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "VelcentAdMobSubsystem.h"

UVelcentAdMobSubsystem* UVelcentAdMobBlueprintLibrary::GetVelcentAdMobSubsystem(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<UVelcentAdMobSubsystem>() : nullptr;
}

void UVelcentAdMobBlueprintLibrary::InitializeAdMob(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->InitializeAdMob();
	}
}

void UVelcentAdMobBlueprintLibrary::ShowDefaultBanner(const UObject* WorldContextObject, EVelcentAdMobBannerPosition Position)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->ShowDefaultBanner(Position);
	}
}

void UVelcentAdMobBlueprintLibrary::HideBanner(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->HideBanner();
	}
}

void UVelcentAdMobBlueprintLibrary::DestroyBanner(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->DestroyBanner();
	}
}

void UVelcentAdMobBlueprintLibrary::LoadDefaultInterstitial(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->LoadDefaultInterstitial();
	}
}

void UVelcentAdMobBlueprintLibrary::ShowInterstitial(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->ShowInterstitial();
	}
}

void UVelcentAdMobBlueprintLibrary::LoadDefaultRewarded(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->LoadDefaultRewarded();
	}
}

void UVelcentAdMobBlueprintLibrary::ShowRewarded(const UObject* WorldContextObject)
{
	if (UVelcentAdMobSubsystem* Subsystem = GetVelcentAdMobSubsystem(WorldContextObject))
	{
		Subsystem->ShowRewarded();
	}
}
