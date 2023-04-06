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
	
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	static TObjectPtr<ASplineTrackCreatorActor> GetInstance();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsMulitpleLaps();

	int GetLaps();
	
	

	
private:
	UFUNCTION()
		void CreateTrack();

	UFUNCTION()
		void ClearTrack();
	

	
private:
	UPROPERTY()
		USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
		USplineComponent* SplineComponent;

	// Start Actor
	UPROPERTY()
		AActor* StartActor;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> StartActorClass;
	
	// EndActor
	UPROPERTY()
		AActor* EndActor;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> EndActorClass;


	UPROPERTY()
		TMap<int32, ACheckpointActor*> CheckpointActors;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CheckpointClass;

	UPROPERTY(EditAnywhere)
		bool UseConstructionScript = true;

	UPROPERTY(EditAnywhere)
		bool bIsMultipleLaps = false;

	UPROPERTY(EditAnywhere)
		int MaxLaps = 1;
	
};
