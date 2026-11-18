#include "VelcentAdMob.h"

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogVelcentAdMob);

class FVelcentAdMobModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		UE_LOG(LogVelcentAdMob, Log, TEXT("VelcentAdMob module started."));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogVelcentAdMob, Log, TEXT("VelcentAdMob module shut down."));
	}
};

IMPLEMENT_MODULE(FVelcentAdMobModule, VelcentAdMob)
