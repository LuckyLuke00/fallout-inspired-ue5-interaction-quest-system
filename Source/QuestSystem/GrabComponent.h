#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"

class UPhysicsHandleComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrabComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Grab")
	bool IsHeld() const { return bIsHeld; }

	UFUNCTION(BlueprintCallable, Category = "Grab")
	double GetHoldDistance() const { return HoldDistance; }

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void TryGrab(UPhysicsHandleComponent* PhysicsHandle, const FTransform& RelativeTransform);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void UpdatePhysicsHandleTargetLocationAndRotation(const FTransform& ActorTransform, const FRotator& AxisRotation, const FVector& Origin);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void AddLocalRotation(const FRotator& DeltaRotation);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void TryRelease();

private:
	bool bIsHeld{ false };
	UPhysicsHandleComponent* PhysicsHandleComponent{ nullptr };
	FTransform RelativePreGrabTransform;

	UPROPERTY(EditAnywhere, Category = "Grab")
	double HoldDistance{ 250.0 };

	void SetColliderResponseChannels() const;
};
