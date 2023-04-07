// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMenuUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "MapButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

bool UPlayMenuUserWidget::Initialize()
{
	Super::Initialize();

	if(VerticalBoxTest == nullptr) return false;
	
	if(PlayButton == nullptr) return false;
	PlayButton->OnClicked.AddDynamic(this, &UPlayMenuUserWidget::LoadSelectedLevel);
	
	return true;
}

void UPlayMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DisplayMap();
}

void UPlayMenuUserWidget::DisplayMap()
{
	VerticalBoxTest->ClearChildren();
	if(MapButtonClass)
	{
		for(auto& MapData : MapArray)
		{
			UMapButtonUserWidget* MapWidget = WidgetTree->ConstructWidget<UMapButtonUserWidget>(MapButtonClass);
			MapWidget->OnClickedDelegate.AddDynamic(this, &UPlayMenuUserWidget::OnButtonClicked);
			MapWidget->MapNameString = MapData.MapName;
			MapWidget->MapDisplayText->SetText(FText::FromString(MapData.MapDisplayName));
			VerticalBoxTest->AddChild(MapWidget);
		}
	}
	

}

void UPlayMenuUserWidget::OnButtonClicked(UMapButtonUserWidget* ButtonUserWidget)
{
	
	if(SelectedMapButton != nullptr)
	{
		
		SelectedMapButton->MapButton->SetIsEnabled(true);
		SelectedMapButton->MapButton->SetBackgroundColor(FLinearColor::White);
	}
	
	SelectedMapButton = ButtonUserWidget;
	SelectedMapButton->MapButton->SetIsEnabled(false);
	SelectedMapButton->MapButton->SetBackgroundColor(FLinearColor::Green);
	
}

void UPlayMenuUserWidget::LoadSelectedLevel()
{
	if(SelectedMapButton != nullptr)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(SelectedMapButton->MapNameString));
	}
}
