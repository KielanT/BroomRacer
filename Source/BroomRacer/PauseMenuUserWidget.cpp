// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"

#include "CustomPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UPauseMenuUserWidget::Initialize()
{
	Super::Initialize();
	
	if(ResumeButton == nullptr) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Resume);
	
	if(RestartButton == nullptr) return false;
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Restart);
	
	if(QuitButton == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Quit);
	
	return true;
}

void UPauseMenuUserWidget::Resume()
{
	if (ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->UnPause();
	}
}

void UPauseMenuUserWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void UPauseMenuUserWidget::Quit()
{

	if (ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->UnPause();
	}
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}
