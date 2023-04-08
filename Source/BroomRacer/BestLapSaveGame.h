// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BestLapSaveGame.generated.h"


UCLASS()
class BROOMRACER_API UBestLapSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UBestLapSaveGame();

	// Data to be saved
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FString, float> MapsSaveData; // MapName, best lap

	// Save file name
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	// Save file index
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};
