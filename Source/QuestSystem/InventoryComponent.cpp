#include "InventoryComponent.h"
#include "InventoryItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddItem(AInventoryItem* Item, int32 Quantity)
{
	if (Inventory.Contains(Item))
	{
		Inventory[Item] += Quantity;
	}
	else
	{
		Inventory.Add(Item, Quantity);
	}

	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::RemoveItem(AInventoryItem* Item, int32 Quantity)
{
	if (Inventory.Contains(Item))
	{
		Inventory[Item] -= Quantity;

		if (Inventory[Item] <= 0)
		{
			Inventory.Remove(Item);
		}
	}

	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::GetItemCount(TSubclassOf<AInventoryItem> ItemClass) const
{
	int32 ItemCount{ 0 };

	for (const auto& Item : Inventory)
	{
		if (Item.Key && Item.Key->GetClass() == ItemClass)
		{
			ItemCount += Item.Value;
		}
	}

	return ItemCount;
}
