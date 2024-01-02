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

bool UObjectiveCollection::CheckCompletion()
{
	// Check if all non-optional objectives are complete
	for (auto& Objective : Objectives)
	{
		if (Objective->IsOptional()) continue;
		if (!Objective->IsComplete() && !Objective->IsFailed()) return false;
	}

	// All non optional objectives are complete, so we can fail all optional objectives
	for (const auto& Objective : Objectives)
	{
		if (!Objective->IsOptional()) continue;
		if (Objective->IsComplete()) continue;

		Objective->SetFailed();
	}

	CallOnCompleted(this);
	OnObjectiveCollectionCompleted.Broadcast();

	return true;
}

UObjectiveBase* UObjectiveCollection::GetNextIncompleteObjective() const
{
	for (auto& Objective : Objectives)
	{
		if (Objective->IsComplete() || Objective->IsActive()) continue;

		return Objective;
	}

	return nullptr;
}

void UObjectiveCollection::ActivateAllObjectives()
{
	for (const auto& Objective : Objectives)
	{
		InitiateObjective(Objective);
	}
}

void UObjectiveCollection::ActivateNextObjective()
{
	UObjectiveBase* NextObjective;

	do
	{
		NextObjective = GetNextIncompleteObjective();
		if (!NextObjective) break;

		InitiateObjective(NextObjective);
	} while (NextObjective->IsOptional());
}

void UObjectiveCollection::OnObjectiveCompleted(UObjectiveBase* Objective)
{
	Objective->OnCompleted.RemoveDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
	ActivateNextObjective();
	CheckCompletion();
}

void UObjectiveCollection::InitiateObjective(UObjectiveBase* Objective)
{
	if (!Objective)
	{
		UE_LOG(LogTemp, Warning, TEXT("UObjectiveCollection::InitiateObjective: Objective is nullptr"));
		return;
	}

	if (Objective->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("UObjectiveCollection::InitiateObjective: Objective is already active"));
		return;
	}

	Objective->ActivateObjective();
	Objective->OnCompleted.AddDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
}
