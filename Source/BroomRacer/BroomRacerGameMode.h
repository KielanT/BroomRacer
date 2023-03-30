// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BroomRacerGameMode.generated.h"

class ACheckpointActor;
UCLASS(minimalapi)
class ABroomRacerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABroomRacerGameMode();

protected:
	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaSeconds) override;

public:
	
	UFUNCTION()
		void RaceFinished();
	
private:
	UPROPERTY()
		TArray<AActor*> CheckpointActors;
};



