#pragma once

#include "CoreMinimal.h"
#include "ObjectiveBase.h"
#include "ObjectiveCollection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveCollectionCompleted);

UCLASS(BlueprintType)
class QUESTSYSTEM_API UObjectiveCollection : public UObjectiveBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Objective")
	TArray<UObjectiveBase*> GetObjectives() const { return Objectives; }

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void AddObjective(UObjectiveBase* Objective) { Objectives.Add(Objective); }

	virtual bool IsComplete_Implementation() const override;
	virtual void ActivateObjective_Implementation() override;

	UPROPERTY(BlueprintAssignable, Category = "Objective")
	FOnObjectiveCollectionCompleted OnObjectiveCollectionCompleted;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	bool bOrderRequired{ false };

private:
	UPROPERTY() // Fix potential stale pointer
		TArray<UObjectiveBase*> Objectives;

	UObjectiveBase* GetNextIncompleteObjective() const;

	void ActivateAllObjectives();
	void ActivateNextObjective();
	void InitiateObjective(UObjectiveBase* Objective);

	UFUNCTION(Category = "Objective")
	void OnObjectiveCompleted(UObjectiveBase* Objective);
	void OnAllObjectivesComplete();
};
