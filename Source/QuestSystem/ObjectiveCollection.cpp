#include "ObjectiveCollection.h"

void UObjectiveCollection::ActivateObjective_Implementation()
{
	Super::ActivateObjective_Implementation();

	if (!bOrderRequired)
	{
		ActivateAllObjectives();
		return;
	}

	ActivateNextObjective();
}

bool UObjectiveCollection::IsComplete_Implementation() const
{
	for (auto& Objective : Objectives)
	{
		if (!Objective->IsComplete()) return false;
	}

	return true;
}

UObjectiveBase* UObjectiveCollection::GetNextIncompleteObjective()
{
	for (auto& Objective : Objectives)
	{
		if (Objective->IsActive()) continue;

		return Objective;
	}

	return nullptr;
}

void UObjectiveCollection::ActivateAllObjectives()
{
	for (auto& Objective : Objectives)
	{
		Objective->ActivateObjective();
		Objective->OnCompleted.AddDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
	}
}

void UObjectiveCollection::ActivateNextObjective()
{
	if (UObjectiveBase* NextObjective{ GetNextIncompleteObjective() }; NextObjective)
	{
		NextObjective->ActivateObjective();
		UE_LOG(LogTemp, Warning, TEXT("Activating objective %s"), *NextObjective->GetName());
		NextObjective->OnCompleted.AddDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
	}
}

void UObjectiveCollection::OnObjectiveCompleted(UObjectiveBase* Objective)
{
	UE_LOG(LogTemp, Warning, TEXT("Objective %s completed"), *Objective->GetName());
	Objective->OnCompleted.RemoveDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);

	ActivateNextObjective();
}
