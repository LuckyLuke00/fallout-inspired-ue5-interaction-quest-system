#include "InventoryItem.h"
#include "GrabComponent.h"
#include "InteractComponent.h"
#include "InventoryComponent.h"

AInventoryItem::AInventoryItem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
}

void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	if (InteractComponent)
	{
		InteractComponent->OnInteract.AddDynamic(this, &AInventoryItem::OnInteract);
	}
}

void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInventoryItem::OnInteract(AActor* Interactor)
{
	// Check if the Interactor has an InventoryComponent
	const auto InventoryComponent{ Interactor->FindComponentByClass<UInventoryComponent>() };
	if (!InventoryComponent) return;

	InventoryComponent->AddItem(this);

	if (bDestroyOnPickup)
	{
		Destroy();
	}
}
