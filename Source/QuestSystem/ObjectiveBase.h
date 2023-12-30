#pragma once

#include "CoreMinimal.h"
#include "ObjectiveBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjectiveBase*, Objective);

UCLASS(Abstract, BlueprintType, Blueprintable)
class QUESTSYSTEM_API UObjectiveBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Objective")
	bool IsActive() const { return bIsActive; };

	UFUNCTION(BlueprintCallable, Category = "Objective")
	bool IsOptional() const { return bIsOptional; };

	UFUNCTION(BlueprintNativeEvent, Category = "Objective")
	void ActivateObjective();
	virtual void ActivateObjective_Implementation() { bIsActive = true; };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	bool IsComplete() const;
	virtual bool IsComplete_Implementation() const PURE_VIRTUAL(UObjectiveBase::IsComplete, return false;);

	FOnObjectiveCompleted OnCompleted;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	bool bIsOptional{ false };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	FText ObjectiveName{ FText::FromString("DefaultObjectiveName") };

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void CallOnCompleted(UObjectiveBase* Objective) { OnCompleted.Broadcast(Objective); };

private:
	bool bIsActive{ false };
};
