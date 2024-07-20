#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"

class UGrabComponent;
class UInteractComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class QUESTSYSTEM_API AInventoryItem : public AActor
{
	GENERATED_BODY()

public:
	AInventoryItem();

protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh{ nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGrabComponent* GrabComponent{ nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UInteractComponent* InteractComponent{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Inventory Item")
	bool bDestroyOnPickup{ true };

	UPROPERTY(EditAnywhere, Category = "Inventory Item")
	int32 ItemAmount{ 1 };

	UFUNCTION()
	virtual void OnInteract(AActor* Interactor);
};
