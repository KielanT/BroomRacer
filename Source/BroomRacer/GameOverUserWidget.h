// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUserWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class BROOMRACER_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void Restart();

	UFUNCTION()
		void Quit();
	
public:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> MissedHoopsText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> TimePenaltyText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> LapTimeText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> BestLapTimeText;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> LapTimeWithPenaltyText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> QuitButton;
};
