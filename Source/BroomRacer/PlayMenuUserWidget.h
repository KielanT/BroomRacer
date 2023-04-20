// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayMenuUserWidget.generated.h"

class ULoadingScreenUserWidget;
class UBorder;
class UCanvasPanel;
class UTextBlock;
class UButton;
class UMapButtonUserWidget;
class UVerticalBox;

// Map data is set in the play menu
// Orginally some of the data would be set in the spline
// However, if the track hasn't finished spawning then some of this data will not be set on the spline
// Each level uses the same blueprint of the spline actor, if each map had seperate blueprints then this would not be an issue

USTRUCT()
struct FMapData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString MapName; // Used to set the map name for changing to the correct map
	
	UPROPERTY(EditAnywhere)
		FString MapDisplayName; // The maps display name

	UPROPERTY(EditAnywhere)
		FString MapDetails; // The map details for displaying in the menu

	UPROPERTY()
		float BestLapTime;

	UPROPERTY(EditAnywhere)
		bool bIsMultipleLaps;

	UPROPERTY(EditAnywhere)
		int MaxLaps;
};

UCLASS()
class BROOMRACER_API UPlayMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	virtual void NativeConstruct() override;
	
private:
	void DisplayMap(); // Displays the map buttons
	void DisplayMapDetails(); // Displays the map details

	UFUNCTION()
		void OnButtonClicked(UMapButtonUserWidget* ButtonUserWidget); // Opens the correct map depending on the button
	
	UFUNCTION()
		void LoadSelectedLevel(); // Loads level

	void LoadLevelAfterTime(); // Calls the level if there was loading screen
	
private:
	UPROPERTY(EditAnywhere)
		TArray<FMapData> MapArray;// Array of the map data
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMapButtonUserWidget> MapButtonClass; // Map button class

	UPROPERTY(EditAnywhere)
		TSubclassOf<ULoadingScreenUserWidget> LoadingScreenClass; // Loading screen class
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UVerticalBox> VerticalBoxTest; 

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> PlayButton; // Used for going to the game

	// Map Display Name
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayName;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> MapDetails;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BestLapTime;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> MapDetailBorder;
	

	UPROPERTY()
		TObjectPtr<UMapButtonUserWidget> SelectedMapButton = nullptr; 

	UPROPERTY(EditAnywhere)
		float MinTimeDelay = 10.0f; // Minimun loading screen delay
	
	FTimerHandle LoadingScreenMinTimer;
};
