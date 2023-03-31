// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "CustomPlayerController.h"

bool UHUDWidget::Initialize()
{
	Super::Initialize();

	if(StartGameTimerText != nullptr) return false;
	if(GameRaceTimerText != nullptr) return false;
	if(PreviousLapTimeText != nullptr) return false;
	if(MaxLapsText != nullptr) return false;
	if(CurrentLapsText != nullptr) return false;
	
	return true;
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	int time = GetWorld()->GetTimerManager().GetTimerRemaining(RaceStartCountDownTimer);
	if(time <= TimerAmount - TIMER_OFFSET)
	{
		FText Text = FText::FromString(FString::FromInt(time));
		StartGameTimerText->SetText(Text);
	}
	else
	{
		FText Text = FText::FromString(FString::FromInt(TimerAmount - TIMER_OFFSET));
		StartGameTimerText->SetText(Text);
	}
}

void UHUDWidget::StartTimer(float timerAmount, ACustomPlayerController* Controller)
{
	CustomPlayerController = Controller;
	TimerAmount = timerAmount + TIMER_OFFSET; // offset to add seconds for the text to be visible
	GetWorld()->GetTimerManager().SetTimer(RaceStartCountDownTimer, this, &UHUDWidget::OnTimerFinished, TimerAmount, false);
}

void UHUDWidget::OnTimerFinished()
{
	StartGameTimerText->SetVisibility(ESlateVisibility::Hidden);
	CustomPlayerController->GetPawn()->EnableInput(CustomPlayerController);
}
