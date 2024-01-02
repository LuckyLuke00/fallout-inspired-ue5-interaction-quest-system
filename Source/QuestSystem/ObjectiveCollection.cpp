#include "ObjectiveCollection.h"

void UObjectiveCollection::ActivateObjective_Implementation()
{
	Super::ActivateObjective_Implementation();

	// If no order is required, but bCanSkipOrderedObjectives is true
	bCanSkipOrderedObjectives = bCanSkipOrderedObjectives && bOrderRequired;

	if (!bOrderRequired || bCanSkipOrderedObjectives)
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

	CallOnCompleted();
	OnObjectiveCollectionCompleted.Broadcast();

	return true;
}

UObjectiveBase* UObjectiveCollection::GetNextIncompleteObjective() const
{
	for (auto& Objective : Objectives)
	{
		if (Objective->IsComplete() || Objective->IsActive()) continue;
		Objective->SetHidden(false);

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

	// Always unhide the first objective
	if (!Objectives.IsEmpty())
	{
		Objectives[0]->SetHidden(false);
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

void UObjectiveCollection::CompletePreviousObjectives(const UObjectiveBase* Objective)
{
	// Optional objectives shouldn't complete previous objectives
	if (!Objective || Objective->IsOptional()) return;

	// Find the index of the objective
	const int32 Index{ Objectives.IndexOfByKey(Objective) };

	for (int32 i{ 0 }; i < Index; ++i)
	{
		if (Objectives[i]->IsComplete() || Objectives[i]->IsOptional()) continue;
		Objectives[i]->CallOnCompleted();
	}
}

void UObjectiveCollection::OnObjectiveCompleted(UObjectiveBase* Objective)
{
	Objective->OnCompleted.RemoveDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
	Objective->SetHidden(false);
	if (bCanSkipOrderedObjectives) CompletePreviousObjectives(Objective);
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

	Objective->SetHidden(bCanSkipOrderedObjectives && !Objective->IsOptional());
	Objective->ActivateObjective();
	Objective->OnCompleted.AddDynamic(this, &UObjectiveCollection::OnObjectiveCompleted);
}
