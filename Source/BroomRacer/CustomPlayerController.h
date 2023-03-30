// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnGameOverInterface.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BROOMRACER_API ACustomPlayerController : public APlayerController, public IOnGameOverInterface
{
	GENERATED_BODY()

public:
	virtual void OnGameOver() override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
		void ChangeWidget(TSubclassOf<UUserWidget> WidgetClass);


	
private:
	UPROPERTY(EditAnywhere, Category ="User Interface")
    		TSubclassOf<UUserWidget> HUDWidgetClass; 

	UPROPERTY(EditAnywhere, Category ="User Interface")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
	TArray<AActor*> CheckpointActors;
	
};
