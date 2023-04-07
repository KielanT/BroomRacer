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
		
		HUD->PreviousLapTimeText->SetVisibility(ESlateVisibility::Hidden);
		
		HUD->MaxLapsText->SetVisibility(ESlateVisibility::Hidden);
		
		HUD->BestTimeText->SetVisibility(ESlateVisibility::Hidden);
		
		HUD->CurrentLapsText->SetVisibility(ESlateVisibility::Hidden);
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
			float Seconds = GetWorld()->GetTimerManager().GetTimerElapsed(handle);
			FString SecondsText = FString::SanitizeFloat(Seconds);

			if(Seconds >= 0)
			{
				
				FString SecondsStr = "Lap Time: " + SecondsText;
				HUD->GameRaceTimerText->SetText(FText::FromString(SecondsStr));
			}
			else
			{
				FString SecondsStr = "Lap Time: -";
				HUD->GameRaceTimerText->SetText(FText::FromString(SecondsStr));
			}
		

			if(bIsMultipleLaps)
			{
				HUD->PreviousLapTimeText->SetVisibility(ESlateVisibility::Visible);
				HUD->MaxLapsText->SetVisibility(ESlateVisibility::Visible);
				HUD->BestTimeText->SetVisibility(ESlateVisibility::Visible);
				HUD->CurrentLapsText->SetVisibility(ESlateVisibility::Visible);
				
				if(PlayerPawn->PreviousLapTime > 0)
				{
					FString Previous = "Previous: " + FString::SanitizeFloat(PlayerPawn->PreviousLapTime);
					HUD->PreviousLapTimeText->SetText(FText::FromString(Previous));
				}
				else
				{
					FString Previous = "Previous: -";
					HUD->PreviousLapTimeText->SetText(FText::FromString(Previous));
				}
				
				
				FString Best = "Best: " + FString::SanitizeFloat(PlayerPawn->BestLapTime);
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
