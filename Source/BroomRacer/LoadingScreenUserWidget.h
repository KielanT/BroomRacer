// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenUserWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BROOMRACER_API ULoadingScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void SetMapImage(FString MapName);

protected:
	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere)
		TMap<FString, TObjectPtr<UImage>> MapImages;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> HintText;

	//UPROPERTY(meta = (BindWidget))
	//	TObjectPtr<UImage> MapDisplay;
};
