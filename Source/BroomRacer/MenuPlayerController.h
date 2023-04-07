// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BROOMRACER_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetCameraActor(AActor* Actor);

	virtual void OnPossess(APawn* InPawn) override;

	void ShowPlayMenu();
	void ShowSettingsMenu();
	
	
protected:
	virtual void BeginPlay() override;

private:
	void ChangeWidget(TSubclassOf<UUserWidget> WidgetClass);
	
private:
	UPROPERTY()
		TObjectPtr<AActor> CameraActor;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CameraClass;

	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> PlayClass;
	
	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> SettingsClass;

	UPROPERTY()
		TObjectPtr<UUserWidget> Widget;

	
};
