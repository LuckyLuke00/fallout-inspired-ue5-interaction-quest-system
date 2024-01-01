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

	// Prevent multiple broadcasts and assignments
	if (Quests.Contains(Quest)) return;

	Quests.AddUnique(Quest);
	OnQuestAdded.Broadcast(Quest);
	Quest->OnQuestBaseCompleted.AddDynamic(this, &UQuestManager::OnQuestBaseCompleted);
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

		AddQuest(Quest);
	}
}

void UQuestManager::OnQuestBaseCompleted(AQuestBase* Quest)
{
	OnQuestCompleted.Broadcast(Quest);
	Quest->OnQuestBaseCompleted.RemoveDynamic(this, &UQuestManager::OnQuestBaseCompleted);
}
