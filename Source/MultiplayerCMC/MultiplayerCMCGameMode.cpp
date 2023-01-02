// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerCMCGameMode.h"
#include "MultiplayerCMCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerCMCGameMode::AMultiplayerCMCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
