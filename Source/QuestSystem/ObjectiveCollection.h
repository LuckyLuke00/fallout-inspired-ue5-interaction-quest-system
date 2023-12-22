#pragma once

#include "CoreMinimal.h"
#include "ObjectiveBase.h"
#include "ObjectiveCollection.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEM_API UObjectiveCollection : public UObjectiveBase
{
	GENERATED_BODY()

public:
	virtual void ActivateObjective_Implementation() override;

	// Adds an objective to the collection
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void AddObjective(UObjectiveBase* Objective) { Objectives.Add(Objective); }

	virtual bool IsComplete() const override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	bool bOrderRequired{ false };

private:

	TArray<UObjectiveBase*> Objectives{ nullptr };

	UObjectiveBase* GetNextIncompleteObjective();

	void ActivateAllObjectives();
	void ActivateNextObjective();

	UFUNCTION(Category = "Objective")
	void OnObjectiveCompleted(UObjectiveBase* Objective);
};
