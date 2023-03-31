// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointActor.generated.h"

class UCapsuleComponent;

UCLASS()
class BROOMRACER_API ACheckpointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);


public:
	UPROPERTY(VisibleAnywhere)
		int CheckpointIndex;

	UPROPERTY()
		TArray<AActor*> ActorsPassedThrough;
	
private:
	UPROPERTY()
		USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CheckpointCollision;
	
	
};
