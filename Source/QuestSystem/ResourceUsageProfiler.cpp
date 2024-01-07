#include "ResourceUsageProfiler.h"
#include "Components/BillboardComponent.h"

AResourceUsageProfiler::AResourceUsageProfiler()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Billboard Comp"));
}

void AResourceUsageProfiler::BeginPlay()
{
	Super::BeginPlay();

	FramesCount = 0;
	StartTime = FPlatformTime::Seconds();

	// Open the PDH query and add the counter
	PdhOpenQuery(nullptr, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), NULL, &cpuTotal);

	UpdateStats();

	// Set a timer to call UpdateStats every second
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AResourceUsageProfiler::UpdateStats, 1.0f, true);
}

void AResourceUsageProfiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	++FramesCount;
}

void AResourceUsageProfiler::UpdateStats()
{
	const double CurrentTime{ FPlatformTime::Seconds() };
	const double ElapsedTime{ CurrentTime - StartTime };

	if (ElapsedTime > 0)
	{
		const double AverageFPS{ FramesCount / ElapsedTime };
		const double FPSChange{ ((AverageFPS - TargetFPS) / TargetFPS) * 100 };

		// Get CPU load and RAM usage
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		const DWORDLONG PhysMemUsedMb{ (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024) };
		const double RAMChange{ ((PhysMemUsedMb - TargetRAMUsage) / TargetRAMUsage) * 100 };

		// Collect the PDH data and get the formatted counter value
		PdhCollectQueryData(cpuQuery);
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, nullptr, &counterVal);
		const double CPULoadChange{ ((counterVal.doubleValue - TargetCPULoad) / TargetCPULoad) * 100 };

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Average CPU Load: %f (%.2f%% change)"), counterVal.doubleValue, CPULoadChange));
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Average RAM Usage: %llu MB (%.2f%% change)"), PhysMemUsedMb, RAMChange));
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Average FPS: %f (%.2f%% change)"), AverageFPS, FPSChange));
	}
}
