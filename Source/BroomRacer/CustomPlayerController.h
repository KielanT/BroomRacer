// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnGameStateInterface.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BROOMRACER_API ACustomPlayerController : public APlayerController, public IOnGameStateInterface
{
	GENERATED_BODY()

public:
	virtual void OnGameStart() override;
	virtual void OnGameOver() override;
	virtual void OnPauseForCutScene() override;
	virtual void OnUnPauseForCutScene() override;

	void SetMaxLaps(int maxLaps);
	void SetCurrentLap(int currentLap);
	void SetMultipleLaps(bool isMultiple);

	void SetPauseMenu();
	void UnPause();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
		void ChangeWidget(TSubclassOf<UUserWidget> WidgetClass);
	
	UFUNCTION()
		void RaceTimer();

	
	
private:
	UPROPERTY(EditAnywhere, Category ="User Interface")
    		TSubclassOf<UUserWidget> HUDWidgetClass; 

	UPROPERTY(EditAnywhere, Category ="User Interface")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, Category ="User Interface")
		TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		TArray<AActor*> CheckpointActors;

	UPROPERTY()
	bool bIsMultipleLaps = false;
	
	UPROPERTY()
		int CurrentLap = 1;

	UPROPERTY()
		int MaxLaps = 0;
};
