#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestBase.generated.h"

class UObjectiveCollection;

UCLASS(Abstract)
class QUESTSYSTEM_API AQuestBase : public AActor
{
	GENERATED_BODY()

public:
	AQuestBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Quest")
	UObjectiveCollection* GetRootObjectiveCollection() const { return RootObjectiveCollection; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	UObjectiveCollection* ConstructRootObjectiveCollection();
	virtual UObjectiveCollection* ConstructRootObjectiveCollection_Implementation() PURE_VIRTUAL(AQuestBase::ConstructRootObjectiveCollection_Implementation, return nullptr;);

	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	void PopulateObjectives(const UObjectiveCollection* ObjectiveCollection);
	virtual void PopulateObjectives_Implementation(const UObjectiveCollection* ObjectiveCollection) PURE_VIRTUAL(AQuestBase::PopulateObjectives_Implementation, );

private:
	UPROPERTY() // Fix potential stale pointer
		UObjectiveCollection* RootObjectiveCollection{ nullptr };
};
