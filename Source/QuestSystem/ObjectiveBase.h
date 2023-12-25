#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectiveBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjectiveBase*, Objective);

UCLASS(Abstract, BlueprintType, Blueprintable)
class QUESTSYSTEM_API UObjectiveBase : public UObject
{
	GENERATED_BODY()

public:
	bool IsActive() const { return bIsActive; };

	UFUNCTION(BlueprintNativeEvent, Category = "Objective")
	void ActivateObjective();
	virtual void ActivateObjective_Implementation() { bIsActive = true; };

	FOnObjectiveCompleted OnCompleted;

	virtual bool IsComplete() const PURE_VIRTUAL(UObjectiveBase::IsComplete, return false;);
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	FText ObjectiveName{ FText::FromString("DefaultObjectiveName") };

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void CallOnCompleted() { OnCompleted.Broadcast(this); };

private:
	bool bIsActive{ false };
};
