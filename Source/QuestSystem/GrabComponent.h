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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Grab")
	bool IsHeld() const { return bIsHeld; }

	UFUNCTION(BlueprintCallable, Category = "Grab")
	double GetHoldDistance() const { return HoldDistance; }

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void TryGrab(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void TryRelease();

private:
	bool bIsHeld{ false };
	UPhysicsHandleComponent* PhysicsHandleComponent{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Grab")
	double HoldDistance{ 250.0 };

	void SetColliderResponseChannels() const;
};
