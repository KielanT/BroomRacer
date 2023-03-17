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

	if(CameraActor)
	{
		auto Actor = GetWorld()->SpawnActor(CameraActor);
		if(TObjectPtr<AMenuPlayerController> Controller = Cast<AMenuPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			if(TObjectPtr<APlayerCharacter> Character = Cast<APlayerCharacter>(Controller->GetPawn()))
			{
				Character->IsInGame = false;
				UE_LOG(LogTemp, Warning, TEXT("Found Character"));
			}
			Controller->SetCameraActor(Actor);
		}
	}
}
