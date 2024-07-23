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
	UFUNCTION(BlueprintPure, Category = "Grab")
	bool IsHeld() const { return bIsHeld; }

	UFUNCTION(BlueprintPure, Category = "Grab")
	double GetHoldDistance() const { return HoldDistance; }

	UFUNCTION(BlueprintPure, Category = "Grab")
	double GetReleaseDistanceM() const { return ReleaseDistanceM; }

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void TryGrab(UPhysicsHandleComponent* PhysicsHandle, const FTransform& RelativeTransform);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void UpdatePhysicsHandleTargetLocationAndRotation(const FTransform& ActorTransform, const FRotator& AxisRotation, const FVector& Origin);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void AddLocalRotation(const FRotator& DeltaRotation);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	bool WasReleaseSpeedExceeded() const;

	UFUNCTION(BlueprintCallable, Category = "Grab")
	bool WasReleaseDistanceExceeded() const;

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void TryRelease();

private:
	UPROPERTY(EditAnywhere, Category = "Grab")
	double HoldDistance{ 250.0 };

	UPROPERTY(EditAnywhere, Category = "Grab")
	double ReleaseSpeedKmH{ 65.0 };

	UPROPERTY(EditAnywhere, Category = "Grab")
	double ReleaseDistanceM{ 1.25 };

	UPROPERTY(EditAnywhere, Category = "Grab", meta = (ClampMin = "0.0"))
	float InterpolationSpeed{ 10.f };

	bool bIsHeld{ false };

	FTransform RelativePreGrabTransform;

	UPhysicsHandleComponent* PhysicsHandleComponent{ nullptr };
	UPrimitiveComponent* GrabbedPrimitiveComponent{ nullptr };

	void SetColliderResponseChannels() const;
};
