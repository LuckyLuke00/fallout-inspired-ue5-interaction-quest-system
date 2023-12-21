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
	// Sets default values for this actor's properties
	AQuestBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Quest")
	const UObjectiveCollection* GetRootObjectiveCollection() { return RootObjectiveCollection; };

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	UObjectiveCollection* ConstructRootObjectiveCollection();
	virtual UObjectiveCollection* ConstructRootObjectiveCollection_Implementation() PURE_VIRTUAL(AQuestBase::ConstructRootObjectiveCollection_Implementation, return nullptr;);

	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	void PopulateObjectives(const UObjectiveCollection* ObjectiveCollection);
	virtual void PopulateObjectives_Implementation(const UObjectiveCollection* ObjectiveCollection) PURE_VIRTUAL(AQuestBase::PopulateObjectives_Implementation, );

private:
	UObjectiveCollection* RootObjectiveCollection{ nullptr };
};
