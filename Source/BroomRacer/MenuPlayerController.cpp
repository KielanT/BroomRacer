// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"

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
