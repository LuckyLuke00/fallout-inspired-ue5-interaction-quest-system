#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestManager.generated.h"

class AQuestBase;

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

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<AQuestBase*> Quests;

	void AddAutoActivatingQuests();
};
