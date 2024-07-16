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

	// Prevent interactables already in range at BeginPlay from not being detected
	TArray<AActor*> OverlappingActors;
	InteractDetectionSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	for (const auto& Actor : OverlappingActors)
	{
		if (const auto InteractComponent{ GetInteractableComponent(Actor) })
		{
			InteractablesInRange.AddUnique(InteractComponent);
			bShouldUpdateClosestInteractable = true;
		}
	}
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

		// If the closest interactable is the one that just left the range, we need to update the closest interactable
		if (ClosestInteractable == InteractComponent)
		{
			ClosestInteractable = nullptr;
			OnInteractDetectionUpdated.Broadcast();
		}

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

	const auto CameraManager{ GetCameraManager() };
	if (!CameraManager) return;

	const FVector StartTrace{ CameraManager->GetCameraLocation() };
	const FVector EndTrace{ StartTrace + CameraManager->GetCameraRotation().Vector() * InteractRange };

	if (const auto Actor{ SphereTrace(StartTrace, EndTrace) })
	{
		if (const auto InteractComponent{ GetInteractableComponent(Actor) })
		{
			ClosestInteractable = InteractComponent;
		}
	}

	// If the closest interactable is still nullptr, it means we're nowhere near any interactables
	if (!ClosestInteractable) return;

	// Then do the most accurate check
	if (const auto Closest{ LineTrace(StartTrace, EndTrace) })
	{
		if (const auto InteractComponent{ GetInteractableComponent(Closest) })
		{
			ClosestInteractable = InteractComponent;
		}
	}
}

AActor* UInteractDetectionComponent::LineTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);


	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams, ResponseParams))
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

AActor* UInteractDetectionComponent::SphereTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

	if (GetWorld()->SweepSingleByChannel(HitResult, StartTrace, EndTrace, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(InteractionToleranceRadius), TraceParams, ResponseParams))
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

APlayerCameraManager* UInteractDetectionComponent::GetCameraManager() const
{
	const APlayerController* PlayerController{ GetWorld()->GetFirstPlayerController() };
	if (!PlayerController) return nullptr;

	return PlayerController->PlayerCameraManager;
}