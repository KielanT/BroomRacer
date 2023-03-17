// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InteractInterface.h"
#include "MenuBroomPawn.generated.h"

class ULevelSequence;
class UBoxComponent;
class UMenuButtonActorWidget;

UENUM()
enum class EMoveState
{
	None = 0,
	FlyOfScreen,
	Controlled,
};

UCLASS()
class BROOMRACER_API AMenuBroomPawn : public APawn, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenuBroomPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact(APawn* InteractCharacter) override;

	UFUNCTION(BlueprintCallable)
		void LoadLevel();
	
private:
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USceneComponent> AttachLocation;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UMenuButtonActorWidget> MenuButtonWidget;
	
	UPROPERTY(EditAnywhere)
		FString ButtonText;

	UPROPERTY(EditAnywhere)
		FString LevelToLoad;

	UPROPERTY(EditAnywhere)
		EMoveState MoveState;
};
