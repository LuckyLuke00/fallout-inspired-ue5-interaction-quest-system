#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractDetectionComponent.generated.h"

class UPrimitiveComponent;
class UInteractComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDetectionUpdated);

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
	UInteractComponent* GetClosestInteractable() const { return ClosestInteractable; }

	UPROPERTY(BlueprintAssignable, Category = "Interactable Detection")
	FOnInteractDetectionUpdated OnInteractDetectionUpdated;

private:
	bool bShouldUpdateClosestInteractable{ true };
	TArray<UInteractComponent*> InteractablesInRange;

	UPROPERTY() // Fix potential stale pointer
		UInteractComponent* ClosestInteractable{ nullptr };
	UPROPERTY() // Fix potential stale pointer
		UPrimitiveComponent* InteractDetectionSphere{ nullptr };

	AActor* LineTrace() const;
	APlayerCameraManager* GetCameraManager() const;
	double GetInteractableLookPercent(const UInteractComponent* Interactable) const;
	static UInteractComponent* GetInteractableComponent(const AActor* Actor);
	void UpdateClosestInteractable();

	UPROPERTY(EditAnywhere, Category = "Interactable Detection", meta = (ClampMin = "0.0"))
	double InteractRange{ 800.0 };

	UPROPERTY(EditAnywhere, Category = "Interactable Detection", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	double LookPercentThreshold{ 0.99 };

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
