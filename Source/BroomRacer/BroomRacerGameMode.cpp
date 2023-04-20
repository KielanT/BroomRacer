// Copyright Epic Games, Inc. All Rights Reserved.

#include "BroomRacerGameMode.h"

#include "CheckpointActor.h"
#include "CustomPlayerController.h"
#include "MainGameInstance.h"
#include "OnGameStateInterface.h"
#include "PlayerBroomPawn.h"
#include "SplineTrackCreatorActor.h"
#include "Components/AudioComponent.h"
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

	// Plays background music
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Background Music"));
}

void ABroomRacerGameMode::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->Play(); // Plays the music

	// Works for testing levels individual, does not work when from level
	//if(const TObjectPtr<ASplineTrackCreatorActor> Track = ASplineTrackCreatorActor::GetInstance())
	//{
	//	bIsMultipleLaps = Track->IsMulitpleLaps();
	//	MaxLaps = Track->GetLaps();
	//}

	// Gets the lap settings from the game instance
	UMainGameInstance* GameInstanceRef = Cast<UMainGameInstance>(GetGameInstance());
	if(GameInstanceRef)
	{
		bIsMultipleLaps = GameInstanceRef->bIsMultipleLaps;
		MaxLaps = GameInstanceRef->MaxLaps;
	}
	
	// Sets the values 
	CustomPlayerController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CustomPlayerController->SetMultipleLaps(bIsMultipleLaps);
	CustomPlayerController->SetMaxLaps(MaxLaps);

	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInWorld);
	// Pauses all actors that implements the game state interface for the cutscene
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
				break; // Currently only one actor implements so break out for performance
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
	// Unpauses all actors 
	for(auto Actor : ActorsInWorld)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(Actor, UOnGameStateInterface::StaticClass()))
		{
			if(IOnGameStateInterface* Interface = Cast<IOnGameStateInterface>(Actor))
			{
				Interface->OnUnPauseForCutScene();
			}
			break;// Currently only one actor implements so break out for performance
		}
	}
}

