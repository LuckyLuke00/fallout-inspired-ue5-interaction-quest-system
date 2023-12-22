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

bool UObjectiveCollection::IsComplete() const
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
	}
}

void UObjectiveCollection::ActivateNextObjective()
{
	if (UObjectiveBase* NextObjective{ GetNextIncompleteObjective() }; NextObjective)
	{
		NextObjective->ActivateObjective();
		NextObjective->OnCompleted.AddDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
	}
}

void UObjectiveCollection::OnObjectiveCompleted(UObjectiveBase* Objective)
{
	Objective->OnCompleted.RemoveDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);

	ActivateNextObjective();
}
