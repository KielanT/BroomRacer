// Copyright Epic Games, Inc. All Rights Reserved.

#include "BroomRacerGameMode.h"

#include "CheckpointActor.h"
#include "OnGameStateInterface.h"
#include "PlayerBroomPawn.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABroomRacerGameMode::ABroomRacerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BroomRacer/Blueprints/Characters/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABroomRacerGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void ABroomRacerGameMode::RaceFinished()
{
	// if multiple laps dont call interface
	TArray<AActor*> ActorsInWorld;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInWorld);
	
	for(auto Actor : ActorsInWorld)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(Actor, UOnGameStateInterface::StaticClass()))
		{
			if(IOnGameStateInterface* Interface = Cast<IOnGameStateInterface>(Actor))
			{
				Interface->OnGameOver();
			}
			break;
		}
		
	}

	
	
}

