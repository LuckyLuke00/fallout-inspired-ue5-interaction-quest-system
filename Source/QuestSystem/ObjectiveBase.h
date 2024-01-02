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

	UFUNCTION(BlueprintCallable, Category = "Objective")
	bool IsFailed() const { return bIsFailed; };

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void SetFailed(bool bFailed = true) { bIsFailed = bFailed; if (bFailed) DeactivateObjective(); };

	UFUNCTION(BlueprintNativeEvent, Category = "Objective")
	void ActivateObjective();
	virtual void ActivateObjective_Implementation() { bIsActive = true; };

	UFUNCTION(BlueprintNativeEvent, Category = "Objective")
	void DeactivateObjective();
	virtual void DeactivateObjective_Implementation() { bIsActive = false; };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	bool IsComplete() const;
	virtual bool IsComplete_Implementation() const { return bIsComplete; };

	UFUNCTION(BlueprintCallable, Category = "Objective")
	const FText& GetObjectiveName() const { return ObjectiveName; };

	FOnObjectiveCompleted OnCompleted;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	bool bIsOptional{ false };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective", Meta = (ExposeOnSpawn = "true"))
	FText ObjectiveName{ FText::FromString("DefaultObjectiveName") };

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void CallOnCompleted(UObjectiveBase* Objective) { bIsComplete = true; DeactivateObjective(); OnCompleted.Broadcast(Objective); };

private:
	bool bIsActive{ false };
	bool bIsComplete{ false };
	bool bIsFailed{ false };
};
