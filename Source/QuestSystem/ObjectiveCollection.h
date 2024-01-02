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
	UPROPERTY(BlueprintAssignable, Category = "Objective")
	FOnObjectiveCollectionCompleted OnObjectiveCollectionCompleted;

	UFUNCTION(BlueprintPure, Category = "Objective")
	const TArray<UObjectiveBase*>& GetObjectives() const { return Objectives; }

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void AddObjective(UObjectiveBase* Objective) { Objectives.Add(Objective); }

	virtual void ActivateObjective_Implementation() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	bool bOrderRequired{ false };

private:
	UPROPERTY() // Fix potential stale pointer
		TArray<UObjectiveBase*> Objectives;

	UFUNCTION(Category = "Objective")
	void OnObjectiveCompleted(UObjectiveBase* Objective);

	bool CheckCompletion();
	UObjectiveBase* GetNextIncompleteObjective() const;

	void ActivateAllObjectives();
	void ActivateNextObjective();
	void InitiateObjective(UObjectiveBase* Objective);
};
