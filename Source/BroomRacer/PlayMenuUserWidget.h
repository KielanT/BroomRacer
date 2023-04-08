// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayMenuUserWidget.generated.h"

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

	UPROPERTY(EditAnywhere)
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

	UFUNCTION()
		void OnButtonClicked(UMapButtonUserWidget* ButtonUserWidget);
	
	UFUNCTION()
		void LoadSelectedLevel();
	
private:
	UPROPERTY(EditAnywhere)
		TArray<FMapData> MapArray;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMapButtonUserWidget> MapButtonClass;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UVerticalBox> VerticalBoxTest;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> PlayButton;

	UPROPERTY()
		TObjectPtr<UMapButtonUserWidget> SelectedMapButton = nullptr;
};
