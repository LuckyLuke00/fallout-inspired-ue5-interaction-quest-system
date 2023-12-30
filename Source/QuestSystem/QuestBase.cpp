#include "QuestBase.h"
#include "Components/BillboardComponent.h"
#include "ObjectiveCollection.h"

AQuestBase::AQuestBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Billboard Comp"));
}

void AQuestBase::BeginPlay()
{
	Super::BeginPlay();

	RootObjectiveCollection = ConstructRootObjectiveCollection();
	PopulateObjectives(RootObjectiveCollection);
	RootObjectiveCollection->ActivateObjective();
}

void AQuestBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
