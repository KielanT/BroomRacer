// Fill out your copyright notice in the Description page of Project Settings.


#include "MapButtonUserWidget.h"

#include "Components/Button.h"

bool UMapButtonUserWidget::Initialize()
{
	Super::Initialize();

	if(MapButton == nullptr) return false;
	MapButton->OnClicked.AddUniqueDynamic(this, &UMapButtonUserWidget::OnButtonClicked);

	return true;
}

void UMapButtonUserWidget::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(this);
}
