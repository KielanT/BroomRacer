// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUserWidget.generated.h"

class UTextBlock;

UCLASS()
class BROOMRACER_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> MissedHoopsText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> TimePenaltyText;
};
