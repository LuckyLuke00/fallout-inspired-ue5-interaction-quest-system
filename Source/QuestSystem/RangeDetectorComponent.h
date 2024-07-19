#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangeDetectorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API URangeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URangeDetectorComponent();

	UFUNCTION(BlueprintCallable, Category = "Range Detection")
	const UActorComponent* FindClosestActorWithComponent(const TSubclassOf<UActorComponent> ComponentClass) const;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(Transient)
	UPrimitiveComponent* RangeDetectionSphere{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Range Detection", meta = (ClampMin = "0.0"))
	float DetectionRange{ 800.f };

	UPROPERTY(EditDefaultsOnly, Category = "Range Detection", meta = (ClampMin = "0.0", ClampMax = "50.0"))
	float DetectionToleranceRadius{ 25.f };

	FCollisionResponseParams CollisionResponseParams;

	void SetDetectionSphereRadius();
	void SetDetectionSphereCollision();
	void AttachDetectionSphere();

	void SetupCollisionResponseParams();

	bool AreAnyActorsInRange() const;
};
