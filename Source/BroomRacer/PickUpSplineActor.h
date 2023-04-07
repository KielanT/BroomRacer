// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpSplineActor.generated.h"

class ABasePickUpActor;
class USplineComponent;

UCLASS()
class BROOMRACER_API APickUpSplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	void SpawnPickUps();
	
private:
	UPROPERTY()
		USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
		USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere)
		TMap<int, TSubclassOf<ABasePickUpActor>> Pickups;
};
