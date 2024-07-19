#include "RangeDetectorComponent.h"
#include "Components/SphereComponent.h"
#include "HelperFunctions.h"

URangeDetectorComponent::URangeDetectorComponent() :
	RangeDetectionSphere{ CreateDefaultSubobject<USphereComponent>(TEXT("InteractDetectionSphere")) }
{
	PrimaryComponentTick.bCanEverTick = false;

	SetDetectionSphereRadius();
	SetDetectionSphereCollision();

	SetupCollisionResponseParams();
}

void URangeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	AttachDetectionSphere();
}

bool URangeDetectorComponent::AreAnyActorsInRange() const
{
	TArray<AActor*> OverlappingActors;
	RangeDetectionSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	return !OverlappingActors.IsEmpty();
}

const UActorComponent* URangeDetectorComponent::FindClosestActorWithComponent(const TSubclassOf<UActorComponent> ComponentClass) const
{
	if (!AreAnyActorsInRange())
		return nullptr;

	const APlayerCameraManager* CameraManager{ UHelperFunctions::GetCameraManager(this) };

	const FVector StartTrace{ CameraManager->GetCameraLocation() };
	const FVector EndTrace{ StartTrace + CameraManager->GetCameraRotation().Vector() * DetectionRange };

	const UActorComponent* ActorComponent{ nullptr };
	if (const auto Actor{ UHelperFunctions::SphereTrace(this, StartTrace, EndTrace, DetectionToleranceRadius, GetOwner(), CollisionResponseParams) })
	{
		ActorComponent = Actor->FindComponentByClass(ComponentClass);
	}

	if (ActorComponent == nullptr)
		return nullptr;

	if (const auto Actor{ UHelperFunctions::LineTrace(this, StartTrace, EndTrace, GetOwner(), CollisionResponseParams) })
	{
		ActorComponent = Actor->FindComponentByClass(ComponentClass);
	}

	return ActorComponent;
}

void URangeDetectorComponent::SetDetectionSphereRadius()
{
	USphereComponent* SphereComponent{ Cast<USphereComponent>(RangeDetectionSphere) };
	if (SphereComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("URangeDetectorComponent::SetSphereRadius - Failed to cast RangeDetectionSphere to USphereComponent"));
		return;
	}

	const float Radius{ DetectionRange * .5f };
	SphereComponent->SetSphereRadius(Radius);
}

void URangeDetectorComponent::SetDetectionSphereCollision()
{
	RangeDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RangeDetectionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	RangeDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RangeDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	RangeDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	RangeDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
}

void URangeDetectorComponent::AttachDetectionSphere()
{
	RangeDetectionSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void URangeDetectorComponent::SetupCollisionResponseParams()
{
	CollisionResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	CollisionResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
}
