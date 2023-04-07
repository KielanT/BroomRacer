// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuActorUserWidget.h"


bool UMenuActorUserWidget::Initialize()
{
	Super::Initialize();

	if(ButtonText == nullptr) return false;

	return true;
}

