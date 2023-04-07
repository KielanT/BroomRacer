// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "CheckpointActor.h"
#include "GameOverUserWidget.h"
#include "HUDWidget.h"
#include "PlayerBroomPawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ACustomPlayerController::OnGameStart()
{
	
}

void ACustomPlayerController::OnGameOver()
{
	GetPawn()->DisableInput(this);
	ChangeWidget(GameOverWidgetClass);
	if(CurrentWidget->GetClass()->IsChildOf(UGameOverUserWidget::StaticClass()))
	{
		if(APlayerBroomPawn* PlayerPawn = Cast<APlayerBroomPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			UGameOverUserWidget* GameOverWidget = Cast<UGameOverUserWidget>(CurrentWidget);
			int numCheckpoints = CheckpointActors.Num();
			if(bIsMultipleLaps)
			{
				numCheckpoints =  CheckpointActors.Num() * (MaxLaps + 1); // Offset of 1 starts count at 0
			}
			
			const int Missed = numCheckpoints - PlayerPawn->CheckpointsPassed;
			if(Missed > 0)
			{
				int penalty = Missed * 5;
				FString MissedText = "Hoops Missed: ";
				MissedText.AppendInt(Missed);

				FString TimePenalty = "Time Penalty: ";
				TimePenalty.AppendInt( penalty);

				FString LapTimeWithPenalty = "Time with Penalty: ";
				LapTimeWithPenalty.AppendInt(PlayerPawn->PreviousLapTime + penalty);
				GameOverWidget->LapTimeWithPenaltyText->SetText(FText::FromString(LapTimeWithPenalty));
				
				GameOverWidget->MissedHoopsText->SetText(FText::FromString(MissedText));
				GameOverWidget->TimePenaltyText->SetText(FText::FromString(TimePenalty));
			}
			
			FString BestLapTime = "Best Time: ";
			BestLapTime.AppendInt(PlayerPawn->BestLapTime);
			GameOverWidget->BestLapTimeText->SetText(FText::FromString(BestLapTime));
			
			FString LapTime = "Lap Time: ";
			LapTime.AppendInt(PlayerPawn->PreviousLapTime);
			GameOverWidget->LapTimeText->SetText(FText::FromString(LapTime));

			
			
		}
	}
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointActor::StaticClass(), CheckpointActors);
	ChangeWidget(HUDWidgetClass);

	GetPawn()->DisableInput(this);
	
	if(CurrentWidget->GetClass()->IsChildOf(UHUDWidget::StaticClass()))
	{
		UHUDWidget* HUD = Cast<UHUDWidget>(CurrentWidget);
		HUD->StartTimer(3, this);
	}
	SetInputMode(FInputModeGameOnly());

	
}

void ACustomPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RaceTimer();
	
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

void ACustomPlayerController::RaceTimer()
{
	if(CurrentWidget->GetClass()->IsChildOf(UHUDWidget::StaticClass()))
	{
		if(APlayerBroomPawn* PlayerPawn = Cast<APlayerBroomPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			UHUDWidget* HUD = Cast<UHUDWidget>(CurrentWidget);
			FTimerHandle handle = PlayerPawn->GetLapTimeHandle();
			int Seconds = GetWorld()->GetTimerManager().GetTimerElapsed(handle);
			FString SecondsText = FString::FromInt(Seconds);
			
			if(Seconds >= 0)
				HUD->GameRaceTimerText->SetText(FText::FromString(SecondsText));

			if(PlayerPawn->PreviousLapTime > 0)
			{
				HUD->PreviousLapTimeText->SetText(FText::FromString(FString::FromInt(PlayerPawn->PreviousLapTime)));
			}else
			{
				HUD->PreviousLapTimeText->SetText(FText::FromString(""));
			}

			if(bIsMultipleLaps)
			{
				FString Best = "Best: " + FString::FromInt(PlayerPawn->BestLapTime);
				HUD->BestTimeText->SetText(FText::FromString(Best));
				
				HUD->CurrentLapsText->SetText(FText::FromString(FString::FromInt(CurrentLap)));
				HUD->MaxLapsText->SetText(FText::FromString(FString("/") + FString::FromInt(MaxLaps)));
			}
		}
	}
}

void ACustomPlayerController::SetMaxLaps(int maxLaps)
{
	MaxLaps = maxLaps;
}

void ACustomPlayerController::SetCurrentLap(int currentLap)
{
	CurrentLap = currentLap;
}

void ACustomPlayerController::SetMultipleLaps(bool isMultiple)
{
	bIsMultipleLaps = isMultiple;
}
