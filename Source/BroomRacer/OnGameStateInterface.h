// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnGameStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UOnGameStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BROOMRACER_API IOnGameStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Interfaces for different states of the game
	UFUNCTION()
		virtual void OnGameStart() = 0;
	
	UFUNCTION()
		virtual void OnGameOver() = 0;

	UFUNCTION()
		virtual void OnPauseForCutScene() = 0;

	UFUNCTION()
		virtual void OnUnPauseForCutScene() = 0;
};
