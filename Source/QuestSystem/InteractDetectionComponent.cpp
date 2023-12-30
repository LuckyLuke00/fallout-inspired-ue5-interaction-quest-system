#include "InteractDetectionComponent.h"
#include "Components/SphereComponent.h"
#include "InteractComponent.h"

UInteractDetectionComponent::UInteractDetectionComponent() :
	InteractDetectionSphere{ CreateDefaultSubobject<USphereComponent>(TEXT("InteractDetectionSphere")) }
{
	PrimaryComponentTick.bCanEverTick = true;

	Cast<USphereComponent>(InteractDetectionSphere)->SetSphereRadius(InteractRange * .5);

	// Set Collision
	InteractDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractDetectionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	InteractDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	InteractDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	InteractDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	InteractDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
}

void UInteractDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractDetectionSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	InteractDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractDetectionComponent::OnOverlapBegin);
	InteractDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractDetectionComponent::OnOverlapEnd);
}

void UInteractDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bShouldUpdateClosestInteractable) return;

	const auto OldClosestInteractable{ ClosestInteractable };
	UpdateClosestInteractable();

	if (OldClosestInteractable != ClosestInteractable)
	{
		OnInteractDetectionUpdated.Broadcast();
	}
}

void UInteractDetectionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto InteractComponent{ GetInteractableComponent(OtherActor) })
	{
		InteractablesInRange.AddUnique(InteractComponent);

		bShouldUpdateClosestInteractable = true;
	}
}
void UInteractDetectionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const auto InteractComponent{ GetInteractableComponent(OtherActor) })
	{
		InteractablesInRange.Remove(InteractComponent);

		if (InteractablesInRange.IsEmpty())
		{
			ClosestInteractable = nullptr;
			bShouldUpdateClosestInteractable = false;
			OnInteractDetectionUpdated.Broadcast();
		}
	}
}

UInteractComponent* UInteractDetectionComponent::GetInteractableComponent(const AActor* Actor)
{
	if (!Actor) return nullptr;
	return Actor->FindComponentByClass<UInteractComponent>();
}

void UInteractDetectionComponent::UpdateClosestInteractable()
{
	ClosestInteractable = nullptr;
	double HighestLookPercent{ 0 };

	for (const auto& InteractComponent : InteractablesInRange)
	{
		if (const auto LookPercent{ GetInteractableLookPercent(InteractComponent) }; LookPercent > HighestLookPercent && LookPercent >= LookPercentThreshold)
		{
			HighestLookPercent = LookPercent;
			ClosestInteractable = InteractComponent;
		}
	}

	// If the closest interactable is still nullptr, it means we're no where near any interactables
	if (!ClosestInteractable) return;

	// Then do the most accurate check
	if (const auto Closest{ LineTrace() })
	{
		if (auto InteractComponent{ GetInteractableComponent(Closest) })
		{
			ClosestInteractable = InteractComponent;
		}
	}
}

AActor* UInteractDetectionComponent::LineTrace() const
{
	const auto CameraManager{ GetCameraManager() };
	if (!CameraManager) return nullptr;

	const FVector StartTrace{ CameraManager->GetCameraLocation() };
	const FVector EndTrace{ StartTrace + CameraManager->GetCameraRotation().Vector() * InteractRange };

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

double UInteractDetectionComponent::GetInteractableLookPercent(const UInteractComponent* Interactable) const
{
	const auto CameraManager{ GetCameraManager() };
	if (!CameraManager) return 0;

	// Get the location of the camera
	const FVector CameraLocation{ CameraManager->GetCameraLocation() };
	const FVector CameraForward{ CameraManager->GetCameraRotation().Vector() };

	// Get the location of the interactable
	const FVector InteractableLocation{ Interactable->GetOwner()->GetActorLocation() };

	// Get the normalized direction from the camera to the interactable
	const FVector CameraToInteractable{ (InteractableLocation - CameraLocation).GetSafeNormal() };

	return FVector::DotProduct(CameraForward, CameraToInteractable);
}

APlayerCameraManager* UInteractDetectionComponent::GetCameraManager() const
{
	const APlayerController* PlayerController{ GetWorld()->GetFirstPlayerController() };
	if (!PlayerController) return nullptr;

	return PlayerController->PlayerCameraManager;
}
