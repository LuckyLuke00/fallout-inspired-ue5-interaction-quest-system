#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AActor*, Interactor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

	// Getters for the interactable name and action
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	FString GetInteractableName() const { return InteractableName; }

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	FString GetInteractableAction() const { return InteractableAction; }

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteract OnInteract;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void CallOnInteract(AActor* Interactor) const { OnInteract.Broadcast(Interactor); }

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Interactable")
	FString InteractableName{ "Interactable" };

	UPROPERTY(EditAnywhere, Category = "Interactable")
	FString InteractableAction{ "Take" };

	void SetColliderResponseChannels();
};
