// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuActorUserWidget.generated.h"


/**
 * 
 */
class UTextBlock;

UCLASS()
class BROOMRACER_API UMenuActorUserWidget : public UUserWidget
{
	GENERATED_BODY()



protected:
	virtual bool Initialize() override;


	
public:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> ButtonText;
};
