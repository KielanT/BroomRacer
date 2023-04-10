// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UGameOverUserWidget::Initialize()
{
	Super::Initialize();

	if(MissedHoopsText		  == nullptr) return false;
	if(TimePenaltyText		  == nullptr) return false;
	if(LapTimeText		      == nullptr) return false;
	if(BestLapTimeText		  == nullptr) return false;
	if(LapTimeWithPenaltyText == nullptr) return false;

	if(RestartButton		  == nullptr) return false;
	RestartButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::Restart);
	
	if(QuitButton			  == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::Quit);

	return true;
}

void UGameOverUserWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void UGameOverUserWidget::Quit()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}
