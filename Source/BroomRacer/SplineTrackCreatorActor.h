// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineTrackCreatorActor.generated.h"

class ACheckpointActor;
class USplineComponent;
UCLASS()
class BROOMRACER_API ASplineTrackCreatorActor : public AActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	ASplineTrackCreatorActor();

	//ASplineTrackCreatorActor(const ASplineTrackCreatorActor& actor) = delete;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// OnContruction works every tick in the editor this allows
	// the track to be visible in the editor
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// The Track creator is a singleton because there only needs to be one track 
	// Per level, this makes it easier to call the track code rather than
	// using the find actors functions
	static TObjectPtr<ASplineTrackCreatorActor> GetInstance();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Used to check if the map has multiple laps
	bool IsMulitpleLaps();

	// Used to get the amount of laps
	int GetLaps();
	
	

	
private:
	UFUNCTION()
		void CreateTrack();

	UFUNCTION()
		void ClearTrack(); // Deletes the track
	

	
private:
	UPROPERTY()
		USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
		USplineComponent* SplineComponent;

	// Start Actor for starting the times
	UPROPERTY()
		AActor* StartActor;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> StartActorClass;
	
	// EndActor for getting the lap time and/or finshing the race
	UPROPERTY()
		AActor* EndActor;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> EndActorClass;


	UPROPERTY()
		// Checkpoint index (checkpoint 1, 2 etc.) Checkpoint actor
		// Stores a map of checkpoint actors with there checkpoint index
		TMap<int32, ACheckpointActor*> CheckpointActors;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CheckpointClass; 

	UPROPERTY(EditAnywhere)
		bool UseConstructionScript = true; // Allows the construction function to be used or disabled in the editor

	UPROPERTY(EditAnywhere)
		bool bIsMultipleLaps = false; // Used to set the multipleLaps

	UPROPERTY(EditAnywhere)
		int MaxLaps = 1; // Used to set the max laps, should never be below 1
	
};
