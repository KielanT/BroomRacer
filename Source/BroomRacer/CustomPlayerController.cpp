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
	ChangeWidget(GameOverWidgetClass);
	if(CurrentWidget->GetClass()->IsChildOf(UGameOverUserWidget::StaticClass()))
	{
		if(APlayerBroomPawn* PlayerPawn = Cast<APlayerBroomPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			UGameOverUserWidget* GameOverWidget = Cast<UGameOverUserWidget>(CurrentWidget);
			const int Missed = CheckpointActors.Num() - PlayerPawn->CheckpointsPassed;
			if(Missed > 0)
			{
				FString MissedText = "Hoops Missed: ";
				MissedText.AppendInt(Missed);

				FString TimePenalty = "Time Penalty: ";
				TimePenalty.AppendInt( Missed * 5);
				
				GameOverWidget->MissedHoopsText->SetText(FText::FromString(MissedText));
				GameOverWidget->TimePenaltyText->SetText(FText::FromString(TimePenalty));
			}
		}
	}
	
	// Disable input
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
	
	
}

void ACustomPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
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
