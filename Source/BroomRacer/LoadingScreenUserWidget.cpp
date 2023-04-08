// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenUserWidget.h"

void ULoadingScreenUserWidget::SetMapImage(FString MapName)
{
	//if(!MapImages.IsEmpty())
	//{
	//	if(MapImages.Find(MapName))
	//	{
	//		MapDisplay = *MapImages.Find(MapName);
	//	}
	//}
}

bool ULoadingScreenUserWidget::Initialize()
{
	Super::Initialize();

	//if(MapDisplay == nullptr) return false;
	if(HintText == nullptr) return false;

	return true;
}

void ULoadingScreenUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Load Image based on map name
	
}
