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

	RootObjectiveCollection->OnObjectiveCollectionCompleted.AddDynamic(this, &AQuestBase::OnRootObjectiveCollectionCompleted);
}

void AQuestBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FText& AQuestBase::GetQuestName() const
{
	return RootObjectiveCollection->GetObjectiveName();
}

void AQuestBase::OnRootObjectiveCollectionCompleted()
{
	OnQuestBaseCompleted.Broadcast(this);
	RootObjectiveCollection->OnObjectiveCollectionCompleted.RemoveDynamic(this, &AQuestBase::OnRootObjectiveCollectionCompleted);
}
