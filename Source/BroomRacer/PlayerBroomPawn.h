// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InteractInterface.h"
#include "InputActionValue.h"
#include "PlayerBroomPawn.generated.h"

class ABroomRacerGameMode;
class UCapsuleComponent;
class UFloatingPawnMovement;
UCLASS()
class BROOMRACER_API APlayerBroomPawn : public APawn, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBroomPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact(APawn* InteractCharacter) override;

	void StartLapTime();
	void StopLapTime();

	UFloatingPawnMovement* GetMovement();

	UFUNCTION()
		FTimerHandle GetLapTimeHandle();
	
private:
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnLapTimerFinished();
	
public:
	UPROPERTY()
		int CheckpointsPassed = 0;

	UPROPERTY()
		float PreviousLapTime = 0;

	UPROPERTY()
		float BestLapTime = 0;

	UPROPERTY()
		float DefaultSpeed;
	
private:
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(EditAnywhere) 
		UStaticMeshComponent* BroomStaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
		UFloatingPawnMovement* FloatingPawnMovement;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<USceneComponent> AttachLocation;
	
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;
	
	UPROPERTY()
		FTimerHandle LapTimeTimer;

	UPROPERTY()
		float MaxLapTime = 3600; // Current Max lap time is an hour


};
