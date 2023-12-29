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
}

UInteractComponent* UInteractDetectionComponent::GetClosestInteractable() const
{
	if (InteractablesInRange.IsEmpty()) return nullptr;

	// First do the most accurate check
	if (const auto ClosestInteractable{ LineTrace() })
	{
		if (auto InteractComponent{ GetInteractableComponent(ClosestInteractable) })
		{
			return InteractComponent;
		}
	}

	// Then do a less accurate check (dot)
	double HighestLookPercent{ 0 };
	UInteractComponent* ClosestInteractable{ nullptr };

	for (const auto& InteractComponent : InteractablesInRange)
	{
		if (const auto LookPercent{ GetInteractableLookPercent(InteractComponent) }; LookPercent > HighestLookPercent && LookPercent >= LookPercentThreshold)
		{
			HighestLookPercent = LookPercent;
			ClosestInteractable = InteractComponent;
		}
	}

	return ClosestInteractable;
}

void UInteractDetectionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto InteractComponent{ GetInteractableComponent(OtherActor) })
	{
		InteractablesInRange.AddUnique(InteractComponent);
	}
}
void UInteractDetectionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const auto InteractComponent{ GetInteractableComponent(OtherActor) })
	{
		InteractablesInRange.Remove(InteractComponent);
	}
}

UInteractComponent* UInteractDetectionComponent::GetInteractableComponent(const AActor* Actor) const
{
	if (!Actor) return nullptr;
	return Actor->FindComponentByClass<UInteractComponent>();
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
