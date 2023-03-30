// Copyright Epic Games, Inc. All Rights Reserved.

#include "BroomRacerGameMode.h"

#include "CheckpointActor.h"
#include "OnGameOverInterface.h"
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointActor::StaticClass(), CheckpointActors);

	
}


void ABroomRacerGameMode::RaceFinished()
{
	if(APlayerBroomPawn* Player = Cast<APlayerBroomPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		int missedHoops = CheckpointActors.Num() - Player->CheckpointsPassed;
	}

	TArray<AActor*> ActorsInWorld;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInWorld);
	
	for(auto Actor : ActorsInWorld)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(Actor, UOnGameOverInterface::StaticClass()))
		{
			if(IOnGameOverInterface* Interface = Cast<IOnGameOverInterface>(Actor))
			{
				Interface->OnGameOver();
			}
			break;
		}
		
	}

	
	
}

