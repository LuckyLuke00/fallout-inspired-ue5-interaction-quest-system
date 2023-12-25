#pragma once

#include "CoreMinimal.h"
#include "ObjectiveBase.h"
#include "ObjectiveCollection.generated.h"

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

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	bool bOrderRequired{ false };

private:
	TArray<UObjectiveBase*> Objectives;
	UObjectiveBase* GetNextIncompleteObjective();

	void ActivateAllObjectives();
	void ActivateNextObjective();

	UFUNCTION(Category = "Objective")
	void OnObjectiveCompleted(UObjectiveBase* Objective);
};
