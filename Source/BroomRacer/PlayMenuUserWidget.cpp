// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMenuUserWidget.h"

#include "BestLapSaveGame.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "MapButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "LoadingScreenUserWidget.h"
#include "MainGameInstance.h"

bool UPlayMenuUserWidget::Initialize()
{
	Super::Initialize();

	if(VerticalBoxTest == nullptr) return false;
	
	if(PlayButton == nullptr) return false;
	PlayButton->OnClicked.AddDynamic(this, &UPlayMenuUserWidget::LoadSelectedLevel);// binds the on button click

	if(MapDetailBorder == nullptr) return false;
	if(DisplayName == nullptr) return false;
	if(MapDetails == nullptr) return false;
	if(BestLapTime == nullptr) return false;

	
	return true;
}

void UPlayMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Displays the map
	DisplayMap();
	MapDetailBorder->SetVisibility(ESlateVisibility::Hidden);

	
	
}

void UPlayMenuUserWidget::DisplayMap()
{
	VerticalBoxTest->ClearChildren();

	// If the map button class exists 
	// creates all the map buttons with the map array
	// sets the map data
	
	if(MapButtonClass)
	{
		int i = 0;
		for(auto& MapData : MapArray)
		{
			UMapButtonUserWidget* MapWidget = WidgetTree->ConstructWidget<UMapButtonUserWidget>(MapButtonClass);
			MapWidget->OnClickedDelegate.AddDynamic(this, &UPlayMenuUserWidget::OnButtonClicked);
			MapWidget->MapNameString = MapData.MapName;
			MapWidget->MapDisplayText->SetText(FText::FromString(MapData.MapDisplayName));
			MapWidget->MapDetailsString = MapData.MapDetails;
			MapWidget->MAPIndex = i;

			// Gets the best lap times for each map
			if(UBestLapSaveGame* LoadMapData = Cast<UBestLapSaveGame>(UGameplayStatics::LoadGameFromSlot("MapSave", 0)))
			{
				if(!LoadMapData->MapsSaveData.IsEmpty())
				{
					if(LoadMapData->MapsSaveData.Find(MapData.MapName))
						MapWidget->BestLapTime = *LoadMapData->MapsSaveData.Find(MapData.MapName);
				}
			}
			VerticalBoxTest->AddChild(MapWidget); // Adds the widget to the menu widget
			i++;
		}
	}

	

}

void UPlayMenuUserWidget::DisplayMapDetails()
{
	if(SelectedMapButton != nullptr)
	{
		// Sets the menu widgets to the selected map button data
		MapDetailBorder->SetVisibility(ESlateVisibility::Visible);
		DisplayName->SetText(SelectedMapButton->MapDisplayText->GetText());
		MapDetails->SetText(FText::FromString(SelectedMapButton->MapDetailsString));
		BestLapTime->SetText(FText::FromString(FString::SanitizeFloat(SelectedMapButton->BestLapTime)));
	}
}

void UPlayMenuUserWidget::OnButtonClicked(UMapButtonUserWidget* ButtonUserWidget)
{

	if(SelectedMapButton != nullptr)
	{
		SelectedMapButton->MapButton->SetIsEnabled(true);
		SelectedMapButton->MapButton->SetBackgroundColor(FLinearColor::White);
	}

	// Sets the new selected button
	SelectedMapButton = ButtonUserWidget;
	SelectedMapButton->MapButton->SetIsEnabled(false);
	SelectedMapButton->MapButton->SetBackgroundColor(FLinearColor::Green);
	DisplayMapDetails(); // Displays the correct map details
	
}

void UPlayMenuUserWidget::LoadSelectedLevel()
{
	if(SelectedMapButton != nullptr)
	{
		/*The screen is not visible even though the widget is detected*/
		//if(LoadingScreenClass)
		//{
		//	ULoadingScreenUserWidget* LoadingWidget = CreateWidget<ULoadingScreenUserWidget>(GetOwningPlayer(), LoadingScreenClass); // Creates widget
		//	if(LoadingWidget)
		//	{
		//		
		//		UE_LOG(LogTemp, Warning, TEXT("Loading Screen put on screen"));
		//		LoadingWidget->SetMapImage(SelectedMapButton->MapNameString);
		//		RemoveFromParent();
		//		LoadingWidget->AddToViewport();
		//		UE_LOG(LogTemp, Warning, TEXT("Code still work"));
		//		GetWorld()->GetTimerManager().SetTimer(LoadingScreenMinTimer, this, &UPlayMenuUserWidget::LoadLevelAfterTime, MinTimeDelay, false);
		//	}
		//}
		//else
		//{
			UGameplayStatics::OpenLevel(GetWorld(), FName(SelectedMapButton->MapNameString)); // opens the new level
		//}
		UMainGameInstance* GameInstanceRef = Cast<UMainGameInstance>(GetGameInstance());
		if(GameInstanceRef)
		{
			// Carry over the map settings to the open level
			GameInstanceRef->bIsMultipleLaps = MapArray[SelectedMapButton->MAPIndex].bIsMultipleLaps;
			GameInstanceRef->MaxLaps = MapArray[SelectedMapButton->MAPIndex].MaxLaps;
		}
	}
}

void UPlayMenuUserWidget::LoadLevelAfterTime()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(SelectedMapButton->MapNameString));
}
