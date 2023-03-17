// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MenuButtonActorWidget.generated.h"

class UMenuActorUserWidget;

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BROOMRACER_API UMenuButtonActorWidget : public UWidgetComponent
{
	GENERATED_BODY()

public:
	//UMenuButtonActorWidget();
	
	virtual void InitWidget() override;

private:
	UPROPERTY()
		TObjectPtr<UMenuActorUserWidget> WidgetInst;

	UPROPERTY(EditAnywhere)
		FString ButtonText;
};
