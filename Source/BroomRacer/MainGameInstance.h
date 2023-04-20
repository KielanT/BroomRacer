// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BROOMRACER_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

		
public:
	// Data to be carried between scenes

	UPROPERTY()
		bool bIsMultipleLaps;

	UPROPERTY()
		int MaxLaps;
};
