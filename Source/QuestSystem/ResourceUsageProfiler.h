#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "pdh.h"
#include "Windows/AllowWindowsPlatformTypes.h"

#include "ResourceUsageProfiler.generated.h"

UCLASS()
class QUESTSYSTEM_API AResourceUsageProfiler : public AActor
{
	GENERATED_BODY()

public:

	AResourceUsageProfiler();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark")
	float TargetCPULoad{ .0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark")
	float TargetRAMUsage{ .0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark")
	float TargetFPS{ .0f };

private:
	double StartTime{ .0 };
	double TotalCPULoad{ .0 };
	double TotalFPS{ .0 };
	double TotalPhysMemUsedMB{ .0 };

	int FramesCount{ 0 };

	PDH_FMT_COUNTERVALUE counterVal{ 0 };
	PDH_HCOUNTER cpuTotal{ nullptr };
	PDH_HQUERY cpuQuery{ nullptr };

	FTimerHandle TimerHandle;

	void UpdateStats();
};
