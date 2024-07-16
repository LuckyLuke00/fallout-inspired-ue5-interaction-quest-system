#include "HelperFunctions.h"

const AActor* UHelperFunctions::LineTrace(const UObject* WorldContextObject, const FVector& StartTrace, const FVector& EndTrace, const AActor* IgnoreActor, const FCollisionResponseParams& ResponseParam)
{
	if (!WorldContextObject) return nullptr;

	const UWorld* World{ WorldContextObject->GetWorld() };
	if (!World) return nullptr;

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;

	if (IgnoreActor != nullptr)
		TraceParams.AddIgnoredActor(IgnoreActor);

	if (World->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams, ResponseParam))
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

const AActor* UHelperFunctions::SphereTrace(const UObject* WorldContextObject, const FVector& StartTrace, const FVector& EndTrace, float Radius, const AActor* IgnoreActor, const FCollisionResponseParams& ResponseParam)
{
	if (!WorldContextObject) return nullptr;

	const UWorld* World{ WorldContextObject->GetWorld() };
	if (!World) return nullptr;

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;

	if (IgnoreActor != nullptr)
		TraceParams.AddIgnoredActor(IgnoreActor);

	if (World->SweepSingleByChannel(HitResult, StartTrace, EndTrace, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(Radius), TraceParams, ResponseParam))
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

const APlayerController* UHelperFunctions::GetLocalPlayerController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	const UWorld* World{ WorldContextObject->GetWorld() };
	if (!World) return nullptr;

	for (FConstPlayerControllerIterator Iterator{ World->GetPlayerControllerIterator() }; Iterator; ++Iterator)
	{
		APlayerController* PlayerController{ Iterator->Get() };
		if (PlayerController->IsLocalPlayerController())
		{
			return PlayerController;
		}
	}

	return nullptr;
}

const APlayerCameraManager* UHelperFunctions::GetCameraManager(const UObject* WorldContextObject)
{
	const APlayerController* PlayerController{ GetLocalPlayerController(WorldContextObject) };
	if (!PlayerController) return nullptr;

	return PlayerController->PlayerCameraManager;
}
