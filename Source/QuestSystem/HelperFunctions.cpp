#include "HelperFunctions.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Kismet/KismetMathLibrary.h>

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

const UCameraComponent* UHelperFunctions::GetActiveCamera(const UObject* WorldContextObject)
{
	const APlayerCameraManager* CameraManager{ GetCameraManager(WorldContextObject) };
	if (!CameraManager) return nullptr;

	const AActor* ViewTarget{ CameraManager->GetViewTarget() };

	TInlineComponentArray<UCameraComponent*> CameraComponents;
	ViewTarget->GetComponents<UCameraComponent>(CameraComponents);

	for (UCameraComponent* CameraComponent : CameraComponents)
	{
		if (CameraComponent->IsActive())
		{
			return CameraComponent;
		}
	}

	return nullptr;
}

FVector UHelperFunctions::GetCameraOffset(const UCameraComponent* CameraComponent)
{
	if (CameraComponent == nullptr)
		return FVector::ZeroVector;

	FVector CameraOffset{ CameraComponent->GetRelativeLocation() };

	// Check if the attach parent is a camera boom (SpringArmComponent)
	if (const USpringArmComponent * SpringArmComponent{ Cast<USpringArmComponent>(CameraComponent->GetAttachParent()) })
	{
		CameraOffset += SpringArmComponent->SocketOffset;
	}
	else if (const FName SocketName{ CameraComponent->GetAttachSocketName() }; SocketName != NAME_None)
	{
		CameraOffset += CameraComponent->GetSocketLocation(SocketName) - CameraComponent->GetAttachParent()->GetComponentLocation();
	}

	return CameraOffset;
}

FTransform UHelperFunctions::CalculateChildRelativeTransform(const FTransform& ParentTransform, const FTransform& ChildTransform)
{
	const FVector ChildRelativeLocation{ UKismetMathLibrary::InverseTransformLocation(ParentTransform, ChildTransform.GetLocation()) };
	const FRotator ChildRelativeRotation{ UKismetMathLibrary::InverseTransformRotation(ParentTransform, ChildTransform.Rotator()) };
	const FVector ChildRelativeScale3D{ ChildTransform.GetScale3D() / ParentTransform.GetScale3D() };

	return FTransform{ ChildRelativeRotation, ChildRelativeLocation, ChildRelativeScale3D };
}

FTransform UHelperFunctions::CalculateChildWorldTransform(const FTransform& ParentTransform, const FTransform& RelativeTransform)
{
	const FVector ChildLocation{ UKismetMathLibrary::TransformLocation(ParentTransform, RelativeTransform.GetLocation()) };
	const FRotator ChildRotation{ UKismetMathLibrary::TransformRotation(ParentTransform, RelativeTransform.Rotator()) };
	const FVector ChildScale3D{ RelativeTransform.GetScale3D() * ParentTransform.GetScale3D() };

	return FTransform{ ChildRotation, ChildLocation, ChildScale3D };
}

FVector UHelperFunctions::CalculateCircularPathOffset(const FRotator& Rotation, double Radius, const FRotator& AxisRotation)
{
	const FVector Offset
	{
		Radius * FMath::Cos(FMath::DegreesToRadians(Rotation.Pitch)),
		0.0,
		Radius * FMath::Sin(FMath::DegreesToRadians(Rotation.Pitch))
	};

	return AxisRotation.RotateVector(Offset);
}

double UHelperFunctions::AngleDegBetweenVectors(const FVector& Vector1, const FVector& Vector2)
{
	const double DotProduct{ FVector::DotProduct(Vector1, Vector2) };
	const double MagnitudeProduct{ Vector1.Size() * Vector2.Size() };
	const double CosineValue{ DotProduct / MagnitudeProduct };

	return FMath::RadiansToDegrees(FMath::Acos(CosineValue));
}
