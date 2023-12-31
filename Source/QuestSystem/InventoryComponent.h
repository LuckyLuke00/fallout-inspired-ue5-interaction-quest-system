#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AInventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(AInventoryItem* Item, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(AInventoryItem* Item, int32 Quantity = 1);

	// Function that returns how many items of a certain class are in the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCount(AInventoryItem* ItemClass) const;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

private:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<AInventoryItem*, int32> Inventory;
};
