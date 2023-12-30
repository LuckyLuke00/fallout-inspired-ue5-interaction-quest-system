#include "QuestManager.h"
#include "QuestBase.h"
#include "Kismet/GameplayStatics.h"

UQuestManager::UQuestManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestManager::AddQuest(AQuestBase* Quest)
{
	if (!Quest)
	{
		UE_LOG(LogTemp, Error, TEXT("Quest is not valid"));
		return;
	}

	Quests.AddUnique(Quest);
}

void UQuestManager::BeginPlay()
{
	Super::BeginPlay();

	AddAutoActivatingQuests();
}

void UQuestManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestManager::AddAutoActivatingQuests()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestBase::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AQuestBase* Quest{ Cast<AQuestBase>(Actor) };
		if (!Quest || !Quest->ShouldAutoActivate()) continue;

		Quests.AddUnique(Quest);
	}
}
