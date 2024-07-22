#include "GrabComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "HelperFunctions.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	SetColliderResponseChannels();
}

void UGrabComponent::TryGrab(UPhysicsHandleComponent* PhysicsHandle, const FTransform& RelativeTransform)
{
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGrabComponent::TryGrab - PhysicsHandle is nullptr"));
		return;
	}

	UPrimitiveComponent* ComponentToGrab{ Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()) };
	if (ComponentToGrab == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGrabComponent::TryGrab - ComponentToGrab is nullptr"));
		return;
	}

	RelativePreGrabTransform = UHelperFunctions::CalculateChildRelativeTransform(RelativeTransform, ComponentToGrab->GetComponentTransform());

	PhysicsHandleComponent = PhysicsHandle;

	PhysicsHandleComponent->SetInterpolationSpeed(InterpolationSpeed);
	PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
		ComponentToGrab,
		NAME_None,
		ComponentToGrab->GetComponentLocation(),
		ComponentToGrab->GetComponentRotation()
	);

	bIsHeld = true;
}

void UGrabComponent::UpdatePhysicsHandleTargetLocationAndRotation(const FTransform& ActorTransform, const FRotator& AxisRotation, const FVector& Origin)
{
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGrabComponent::UpdatePhysicsHandleTargetLocationAndRotation - PhysicsHandleComponent is nullptr"));
		return;
	}

	const FVector CircularPath{ UHelperFunctions::CalculateCircularPathOffset(ActorTransform.Rotator(), HoldDistance, AxisRotation) };
	const FVector ForwardVector{ ActorTransform.GetRotation().GetForwardVector() };

	const FVector TargetLocation{ CircularPath + Origin };
	const FRotator WorldRotation{ UHelperFunctions::CalculateChildWorldTransform(ActorTransform, RelativePreGrabTransform).Rotator() };

	PhysicsHandleComponent->SetTargetLocationAndRotation(TargetLocation, WorldRotation);
}

void UGrabComponent::AddLocalRotation(const FRotator& DeltaRotation)
{
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGrabComponent::AddLocalRotation - PhysicsHandleComponent is nullptr"));
		return;
	}

	if (DeltaRotation.IsZero())
		return;

	RelativePreGrabTransform.SetRotation(DeltaRotation.Quaternion() * RelativePreGrabTransform.GetRotation());
}

bool UGrabComponent::WasReleaseSpeedExceeded() const
{
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGrabComponent::WasReleaseSpeedExceeded - PhysicsHandleComponent is nullptr"));
		return false;
	}

	// Convert velocity (cm/s^2) to km/h^2 using 0.001296.
	const double SpeedKmH{ PhysicsHandleComponent->GetGrabbedComponent()->GetPhysicsLinearVelocity().SizeSquared() * (0.001296) };
	return SpeedKmH > ReleaseSpeedKmH * ReleaseSpeedKmH;
}

void UGrabComponent::TryRelease()
{
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGrabComponent::TryGrab - PhysicsHandleComponent is nullptr"));
		return;
	}

	if (!bIsHeld) return;

	PhysicsHandleComponent->ReleaseComponent();

	bIsHeld = false;
}

void UGrabComponent::SetColliderResponseChannels() const
{
	const auto Collider{ Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass())) };
	if (!Collider) return;

	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

	Collider->SetGenerateOverlapEvents(true);
}
