// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapButtonUserWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedSignature, class UMapButtonUserWidget*, ButtonOwner);

UCLASS()
class BROOMRACER_API UMapButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	FOnClickedSignature OnClickedDelegate;
	
public:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> MapButton;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> MapDisplayText;

	UPROPERTY()
		FString MapNameString;

	UPROPERTY()
		FString MapDetailsString;

	UPROPERTY()
		float BestLapTime;

	UPROPERTY()
	int MAPIndex;
	
protected:
	UFUNCTION()
		void OnButtonClicked();
};
