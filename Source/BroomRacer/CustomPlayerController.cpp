// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACustomPlayerController::OnGameOver()
{
	ChangeWidget(GameOverWidgetClass);
	// Disable input
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeWidget(HUDWidgetClass);
}

void ACustomPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ACustomPlayerController::ChangeWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (CurrentWidget != nullptr) // Checks if  widget is null or not
	{
		CurrentWidget->RemoveFromParent(); // Removes widget from viewport
		CurrentWidget = nullptr; // Sets widget to null
	}
	if (WidgetClass != nullptr) // Checks if widget class is null or not
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass); // Creates widget
		if (CurrentWidget != nullptr) 
		{
			CurrentWidget->AddToViewport(); // Adds widget to viewport
		}
	}
}
