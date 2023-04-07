// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUpActor.h"
#include "SpeedBoostPickUpActor.generated.h"

class APlayerBroomPawn;
/**
 * 
 */
UCLASS()
class BROOMRACER_API ASpeedBoostPickUpActor : public ABasePickUpActor
{
	GENERATED_BODY()


protected:
	virtual void OnPickUp(APawn* Pawn) override;

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult) override;

private:
	void OnTimerFinished();
	
private:
	UPROPERTY(EditAnywhere)
		float SpeedBoostRate = 10.0f;
	
	FTimerHandle SpeedBoostTimer;
	
	UPROPERTY()
		TObjectPtr<APlayerBroomPawn> BroomPawn;

	UPROPERTY(EditAnywhere)
		float SpeedMultiplier = 2.0f;
	
	UPROPERTY()
		float PreviousSpeed;
};
