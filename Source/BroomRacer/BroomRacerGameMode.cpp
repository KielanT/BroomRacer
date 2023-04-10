// Copyright Epic Games, Inc. All Rights Reserved.

#include "BroomRacerGameMode.h"

#include "CheckpointActor.h"
#include "CustomPlayerController.h"
#include "OnGameStateInterface.h"
#include "PlayerBroomPawn.h"
#include "SplineTrackCreatorActor.h"
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

	PrimaryActorTick.bCanEverTick = true;
}

void ABroomRacerGameMode::BeginPlay()
{
	Super::BeginPlay();


	
	if(const TObjectPtr<ASplineTrackCreatorActor> Track = ASplineTrackCreatorActor::GetInstance())
	{
		bIsMultipleLaps = Track->IsMulitpleLaps();
		MaxLaps = Track->GetLaps();
	}

	CustomPlayerController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CustomPlayerController->SetMultipleLaps(bIsMultipleLaps);
	CustomPlayerController->SetMaxLaps(MaxLaps);

	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInWorld);
	
	for(auto Actor : ActorsInWorld)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(Actor, UOnGameStateInterface::StaticClass()))
		{
			if(IOnGameStateInterface* Interface = Cast<IOnGameStateInterface>(Actor))
			{
				Interface->OnPauseForCutScene();
			}
			break;
		}
	}
	
}

void ABroomRacerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}


void ABroomRacerGameMode::RaceFinished()
{
	// if multiple laps dont call interface until max laps is reached
	if(!bIsMultipleLaps || CurrentLaps == MaxLaps)
	{
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInWorld);
	
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
	else
	{
		CurrentLaps++;
		CustomPlayerController->SetCurrentLap(CurrentLaps);
	}
	
	
}

void ABroomRacerGameMode::UnPauseAfterCutscene()
{
	for(auto Actor : ActorsInWorld)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(Actor, UOnGameStateInterface::StaticClass()))
		{
			if(IOnGameStateInterface* Interface = Cast<IOnGameStateInterface>(Actor))
			{
				Interface->OnUnPauseForCutScene();
			}
			break;
		}
	}
}

