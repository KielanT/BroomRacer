// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BroomRacerGameMode.generated.h"

class ACustomPlayerController;
class ACheckpointActor;
UCLASS(minimalapi)
class ABroomRacerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABroomRacerGameMode();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	
	UFUNCTION()
		void RaceFinished();

private:
	UPROPERTY()
		int MaxLaps = 0;

	UPROPERTY()
		int CurrentLaps = 0;

	UPROPERTY()
		bool bIsMultipleLaps = false;

	UPROPERTY()
		ACustomPlayerController* CustomPlayerController;

};



