#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUESTSYSTEM_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

	// Getters for the interactable name and action
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	FName GetInteractableName() const { return InteractableName; }

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	FName GetInteractableAction() const { return InteractableAction; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void Interact(AActor* Interactor);
	virtual void Interact_Implementation(AActor* Interactor) PURE_VIRTUAL(UInteractComponent::Interact_Implementation, );

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Interactable")
	FName InteractableName{ "Interactable" };

	UPROPERTY(EditAnywhere, Category = "Interactable")
	FName InteractableAction{ "Take" };

	void SetColliderResponseChannels();
};