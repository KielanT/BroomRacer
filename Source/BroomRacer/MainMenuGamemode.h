// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGamemode.generated.h"

/**
 * 
 */
UCLASS()
class BROOMRACER_API AMainMenuGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGamemode();

protected:
	virtual void BeginPlay() override;
	
};
