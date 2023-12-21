// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemGameMode.h"
#include "QuestSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuestSystemGameMode::AQuestSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
