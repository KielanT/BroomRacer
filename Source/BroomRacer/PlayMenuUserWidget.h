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

USTRUCT()
struct FMapData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString MapName;
	
	UPROPERTY(EditAnywhere)
		FString MapDisplayName;

	UPROPERTY(EditAnywhere)
		FString MapDetails;

	UPROPERTY()
		float BestLapTime;
};

UCLASS()
class BROOMRACER_API UPlayMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	virtual void NativeConstruct() override;
	
private:
	void DisplayMap();
	void DisplayMapDetails();

	UFUNCTION()
		void OnButtonClicked(UMapButtonUserWidget* ButtonUserWidget);
	
	UFUNCTION()
		void LoadSelectedLevel();

	void LoadLevelAfterTime();
	
private:
	UPROPERTY(EditAnywhere)
		TArray<FMapData> MapArray;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMapButtonUserWidget> MapButtonClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ULoadingScreenUserWidget> LoadingScreenClass;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UVerticalBox> VerticalBoxTest;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> PlayButton;

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
		float MinTimeDelay = 10.0f;
	
	FTimerHandle LoadingScreenMinTimer;
};
