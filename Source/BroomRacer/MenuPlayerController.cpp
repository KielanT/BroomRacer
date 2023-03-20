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

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Here for testing

	//if(SettingsClass)
	//{
	//	Widget = CreateWidget(GetWorld(), SettingsClass);
	//	if(Widget)
	//	{
	//		Widget->AddToViewport();
	//	}
	//}
}
