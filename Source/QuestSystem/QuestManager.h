#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestManager.generated.h"

class AQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAdded, AQuestBase*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, AQuestBase*, Quest);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestManager();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AddQuest(AQuestBase* Quest);

	UFUNCTION(BlueprintPure, Category = "Quest")
	const TArray<AQuestBase*>& GetQuests() const { return Quests; }

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool HasQuest(AQuestBase* Quest) const { return Quests.Contains(Quest); }

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestAdded OnQuestAdded;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestCompleted OnQuestCompleted;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<AQuestBase*> Quests;

	void AddAutoActivatingQuests();

	UFUNCTION()
	void OnQuestBaseCompleted(AQuestBase* Quest);
};
