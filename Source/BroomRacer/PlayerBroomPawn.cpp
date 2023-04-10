// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBroomPawn.h"


#include "BestLapSaveGame.h"
#include "CheckpointActor.h"
#include "CustomPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerBroomPawn::APlayerBroomPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	SetRootComponent(CapsuleComponent);
	
	BroomStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Broom Mesh"));
	BroomStaticMesh->SetupAttachment(RootComponent);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeRotation(FRotator(0, -10.0f, 0));
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	Camera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	Camera->PostProcessSettings.bOverride_MotionBlurMax = true;
	Camera->PostProcessSettings.MotionBlurAmount = 0.5f;
	Camera->PostProcessSettings.MotionBlurMax = 2.0f;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement"));
	FloatingPawnMovement->MaxSpeed =  6705.6f; // 150mph since unreal engine units are cm/s
	FloatingPawnMovement->Acceleration = 1000.0f; 
	FloatingPawnMovement->Deceleration = FloatingPawnMovement->Acceleration * DecelerationMultiply;
	FloatingPawnMovement->TurningBoost = 5.0f;
	DefaultSpeed = FloatingPawnMovement->MaxSpeed;
	DefualtAcceleration = FloatingPawnMovement->Acceleration;
	MaxAcceleration = FloatingPawnMovement->Acceleration * 3.0f;
	
	AttachLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Location"));
	AttachLocation->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Col"));
	BoxCollision->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->bAlwaysPlay = false;
}

// Called when the game starts or when spawned
void APlayerBroomPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if(UBestLapSaveGame* LoadMapData = Cast<UBestLapSaveGame>(UGameplayStatics::LoadGameFromSlot("MapSave", 0)))
	{
		if(!LoadMapData->MapsSaveData.IsEmpty())
		{
			const FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

			if(LoadMapData->MapsSaveData.Find(MapName))
				BestLapTime = *LoadMapData->MapsSaveData.Find(MapName);
			
		}
	}
	
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerBroomPawn::OnComponentOverlap);
	
}

// Called every frame
void APlayerBroomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpeedEffectFromFOV();
}

// Called to bind functionality to input
void APlayerBroomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerBroomPawn::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerBroomPawn::Look);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &APlayerBroomPawn::Pause);

		EnhancedInputComponent->BindAction(AccelerationAction, ETriggerEvent::Triggered, this, &APlayerBroomPawn::Acceleration);
		EnhancedInputComponent->BindAction(AccelerationAction, ETriggerEvent::Completed, this, &APlayerBroomPawn::OnAccelerationRelease);
		
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &APlayerBroomPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &APlayerBroomPawn::OnRelease);
		

	}
}

void APlayerBroomPawn::Interact(APawn* InteractCharacter)
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepWorldTransform;
	InteractCharacter->AttachToActor(this, Rules);
	InteractCharacter->SetActorLocation(AttachLocation->GetComponentLocation());
	InteractCharacter->SetActorRotation(AttachLocation->GetComponentRotation());
	InteractCharacter->SetActorEnableCollision(false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
}

void APlayerBroomPawn::StartLapTime()
{
	GetWorld()->GetTimerManager().ClearTimer(LapTimeTimer); // Make sure lap timer is stopped

	GetWorld()->GetTimerManager().SetTimer(LapTimeTimer, this, &APlayerBroomPawn::OnLapTimerFinished, MaxLapTime, false);
}

void APlayerBroomPawn::StopLapTime()
{
	// Set previous Lap time and fastest lap time
	PreviousLapTime = GetWorld()->GetTimerManager().GetTimerElapsed(LapTimeTimer);
	UE_LOG(LogTemp, Warning, TEXT("prev: %f"), PreviousLapTime);
	if(PreviousLapTime < BestLapTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Best: %f"), BestLapTime);
		BestLapTime = PreviousLapTime;
	}
	else if(BestLapTime == 0)
	{
		BestLapTime = PreviousLapTime;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(LapTimeTimer);
}

UFloatingPawnMovement* APlayerBroomPawn::GetMovement()
{
	return FloatingPawnMovement;
}

FTimerHandle APlayerBroomPawn::GetLapTimeHandle()
{
	return LapTimeTimer;
}

float APlayerBroomPawn::GetSpeedInMPH()
{
	const float MPHMultiplier = 44.704;
	return FloatingPawnMovement->Velocity.Size() / MPHMultiplier;
}

void APlayerBroomPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		// add movement
		if( MovementVector.Y > 0.1f)
			AddMovementInput(ForwardDirection, MovementVector.Y);

		AddMovementInput(RightDirection, MovementVector.X);
		//AddControllerYawInput(MovementVector.X);
	}
}

void APlayerBroomPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerBroomPawn::Pause(const FInputActionValue& Value)
{
	if (ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(Controller))
	{
		PlayerController->SetPauseMenu();
	}
	
}

void APlayerBroomPawn::Acceleration(const FInputActionValue& Value)
{
	float Accel = Value.Get<float>();
	if (Controller != nullptr)
	{
		//FloatingPawnMovement->Acceleration = MaxAcceleration * Accel;
		
		FVector CurrentVelocity = FloatingPawnMovement->Velocity;
		const FVector MovementDir = CurrentVelocity.GetSafeNormal();

		float AccelSpeed = 100.0f;
		FVector AccelVec = MovementDir * AccelSpeed * Accel;
		FloatingPawnMovement->Acceleration = AccelVec.Size() / GetWorld()->DeltaTimeSeconds;
		
	}
}

void APlayerBroomPawn::OnAccelerationRelease(const FInputActionValue& Value)
{
	FloatingPawnMovement->Acceleration = DefualtAcceleration;
}

void APlayerBroomPawn::Brake(const FInputActionValue& Value)
{
	float Brake = Value.Get<float>();
	if (Controller != nullptr)
	{
		FVector CurrentVelocity = FloatingPawnMovement->Velocity;
		const FVector MovementDir = CurrentVelocity.GetSafeNormal();


		if(CurrentVelocity.IsNearlyZero())
		{
			return;
		}

		float BrackingFactor = 0.1f;
		FVector BrakingVec = -MovementDir * (FloatingPawnMovement->MaxSpeed / 10.0f) * BrackingFactor;
		FloatingPawnMovement->Deceleration = BrakingVec.Size() / GetWorld()->DeltaTimeSeconds;
		
	}
}

void APlayerBroomPawn::OnRelease(const FInputActionValue& Value)
{
	FloatingPawnMovement->Deceleration = DefualtAcceleration * DecelerationMultiply;
}

void APlayerBroomPawn::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->IsChildOf(ACheckpointActor::StaticClass()))
	{
		// Stops calling for each component on other actor
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			AudioComponent->Play();
		}
	}
}

void APlayerBroomPawn::OnLapTimerFinished()
{
	// Should be rarely called
	// Bring up some sort of "Are you there menu"
}

void APlayerBroomPawn::SpeedEffectFromFOV()
{
	float CurrentSpeed = FloatingPawnMovement->Velocity.Size();
	float SpeedFOV = FMath::Lerp(90.0f, 110.0f, CurrentSpeed / FloatingPawnMovement->MaxSpeed);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->PlayerCameraManager->SetFOV(SpeedFOV);
	}

	
}

