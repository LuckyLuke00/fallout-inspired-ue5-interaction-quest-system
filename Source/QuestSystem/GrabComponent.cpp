#include "GrabComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	SetColliderResponseChannels();
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsHeld || PhysicsHandleComponent == nullptr) return;
}

void UGrabComponent::TryGrab(UPhysicsHandleComponent* PhysicsHandle)
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

	PhysicsHandleComponent = PhysicsHandle;
	PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
		ComponentToGrab,
		NAME_None,
		ComponentToGrab->GetComponentLocation(),
		ComponentToGrab->GetComponentRotation()
	);

	bIsHeld = true;
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
