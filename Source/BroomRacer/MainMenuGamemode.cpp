// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGamemode.h"

#include "MenuPlayerController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGamemode::AMainMenuGamemode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BroomRacer/Blueprints/Characters/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMainMenuGamemode::BeginPlay()
{
	Super::BeginPlay();

}
