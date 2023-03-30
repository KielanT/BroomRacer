// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUserWidget.h"

bool UGameOverUserWidget::Initialize()
{
	Super::Initialize();

	if(MissedHoopsText != nullptr) return false;
	if(TimePenaltyText != nullptr) return false;

	return true;
}