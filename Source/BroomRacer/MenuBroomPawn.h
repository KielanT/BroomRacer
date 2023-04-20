// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InteractInterface.h"
#include "InputActionValue.h"
#include "MenuBroomPawn.generated.h"

// Forward decleration
class ULevelSequence;
class UBoxComponent;
class UMenuButtonActorWidget;

// Moves states
UENUM()
enum class EMoveState
{
	None = 0,
	FlyOfScreen, // Fly of screen when interected
	Controlled, // Move up down with the cursor while using menu
};

UENUM()
enum class EButtonType
{
	None = 0,
	Play,
	Settings,
	Quit
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
		void UnMount();

	UFUNCTION(BlueprintCallable)
		void LoadLevel();

	EButtonType GetButtonType();

	
	
private:
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void Move(const FInputActionValue& Value);
	
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

	UPROPERTY(EditAnywhere)
		EButtonType ButtonType;
	
	UPROPERTY()
		FSlateColor StartSlateColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY()
		APawn* MountedPawn;

	UPROPERTY()
		FVector MountedPawnLocation;
	
	UPROPERTY()
		FRotator MountedPawnRotation;
};
