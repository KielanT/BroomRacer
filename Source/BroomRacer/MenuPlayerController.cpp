// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMenuPlayerController::SetCameraActor(AActor* Actor)
{
	if(Actor)
	{
		CameraActor = Actor;
		SetViewTarget(CameraActor);
	}
}

void AMenuPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(CameraActor)
		SetViewTarget(CameraActor);
}

void AMenuPlayerController::ShowPlayMenu()
{
	ChangeWidget(PlayClass);
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
}

void AMenuPlayerController::ShowSettingsMenu()
{
	ChangeWidget(SettingsClass);
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(CameraClass)
	{
		CameraActor = GetWorld()->SpawnActor(CameraClass);
		SetViewTarget(CameraActor);
	}
}


void AMenuPlayerController::ChangeWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (Widget != nullptr) // Checks if  widget is null or not
	{
		Widget->RemoveFromParent(); // Removes widget from viewport
		Widget = nullptr; // Sets widget to null
	}
	if (WidgetClass != nullptr) // Checks if widget class is null or not
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass); // Creates widget
		if (Widget != nullptr) 
		{
			Widget->AddToViewport(); // Adds widget to viewport
		}
	}
}
