// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuUserWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class BROOMRACER_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void Resume();

	UFUNCTION()
		void Restart();

	UFUNCTION()
		void Quit();

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> QuitButton;
};
