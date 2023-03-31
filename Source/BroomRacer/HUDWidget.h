// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class ACustomPlayerController;

UCLASS()
class BROOMRACER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void StartTimer(float timerAmount, ACustomPlayerController* Controller);

private:
	void OnTimerFinished();

	
public:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> StartGameTimerText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> GameRaceTimerText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> PreviousLapTimeText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> BestTimeText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> MaxLapsText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> CurrentLapsText;

private:
	UPROPERTY()
		FTimerHandle RaceStartCountDownTimer;

	UPROPERTY()
		ACustomPlayerController* CustomPlayerController;

	int TimerAmount = 0;

	const int TIMER_OFFSET = 1;
};
