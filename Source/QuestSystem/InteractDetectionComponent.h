#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractDetectionComponent.generated.h"

class UPrimitiveComponent;
class UInteractComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API UInteractDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractDetectionComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interactable Detection")
	UInteractComponent* GetClosestInteractable() const;

private:
	UPrimitiveComponent* InteractDetectionSphere{ nullptr };

	TArray<UInteractComponent*> InteractablesInRange;

	UPROPERTY(EditAnywhere, Category = "Interactable Detection", meta = (ClampMin = "0.0"))
	double InteractRange{ 800.0 };

	UPROPERTY(EditAnywhere, Category = "Interactable Detection", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	double LookPercentThreshold{ 0.99 };

	// Begin overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// End overlap
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UInteractComponent* GetInteractableComponent(const AActor* Actor) const;
	AActor* LineTrace() const;
	double GetInteractableLookPercent(const UInteractComponent* Interactable) const;

	// Get the camera manager
	APlayerCameraManager* GetCameraManager() const;
};
