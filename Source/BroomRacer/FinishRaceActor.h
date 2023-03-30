// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishRaceActor.generated.h"

class ABroomRacerGameMode;
class UBoxComponent;
UCLASS()
class BROOMRACER_API AFinishRaceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishRaceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult); 

private:
	UPROPERTY()
		USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BlockLeft;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BlockRight;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BlockTop;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BlockBottom;

	UPROPERTY(EditAnywhere)
		UBoxComponent* FinishCollisionBox;

	UPROPERTY()
		ABroomRacerGameMode* GameModeRef;

	bool IsRaceFinished = false;
};
