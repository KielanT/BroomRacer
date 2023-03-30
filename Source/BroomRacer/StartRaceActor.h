// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartRaceActor.generated.h"

UCLASS()
class BROOMRACER_API AStartRaceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartRaceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
		USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;
	

};
