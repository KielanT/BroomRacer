// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBroomPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MenuButtonActorWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "MenuActorUserWidget.h"
#include "MenuPlayerController.h"
#include "PlayerCharacter.h"
#include "Components/TextBlock.h"

// Sets default values
AMenuBroomPawn::AMenuBroomPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Broom Mesh"));
	SetRootComponent(StaticMeshComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Collision Box"));
	BoxCollision->SetupAttachment(RootComponent);

	AttachLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Location"));
	AttachLocation->SetupAttachment(RootComponent);
	
	MenuButtonWidget = CreateDefaultSubobject<UMenuButtonActorWidget>(TEXT("Widget Component"));
	MenuButtonWidget->SetupAttachment(RootComponent);
	MenuButtonWidget->SetVisibility(true);
	
}

// Called when the game starts or when spawned
void AMenuBroomPawn::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMenuBroomPawn::OnComponentOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMenuBroomPawn::OnComponentEndOverlap);

	if(TObjectPtr<UMenuActorUserWidget> MenuWidget = Cast<UMenuActorUserWidget>(MenuButtonWidget->GetWidget()))
	{
		StartSlateColor = MenuWidget->ButtonText->GetColorAndOpacity();
	}
}

// Called every frame
void AMenuBroomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMenuBroomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMenuBroomPawn::Move);
	}
	
}

void AMenuBroomPawn::Interact(APawn* InteractCharacter)
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepWorldTransform;
	InteractCharacter->AttachToActor(this, Rules);
	InteractCharacter->SetActorLocation(AttachLocation->GetComponentLocation());
	InteractCharacter->SetActorRotation(AttachLocation->GetComponentRotation());
	AMenuPlayerController* MenuController = Cast<AMenuPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MenuController->Possess(this);
	MenuButtonWidget->SetVisibility(false);

	if (AMenuPlayerController* BroomController = Cast<AMenuPlayerController>(Controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Broom controller"));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(BroomController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if(MoveState == EMoveState::FlyOfScreen)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(),OutActors);
		ALevelSequenceActor* LevelSequenceActor = nullptr;
		for(auto Actor : OutActors)
		{
			if(Actor->ActorHasTag(FName(ButtonText)))
			{
				LevelSequenceActor = Cast<ALevelSequenceActor>(Actor);
				break;
			}
		}

		if(LevelSequenceActor)
		{
			LevelSequenceActor->SequencePlayer->Play();
		}
	}

	if(ButtonType == EButtonType::Play)
	{
		MenuController->ShowPlayMenu();
	}
	if(ButtonType == EButtonType::Settings)
	{
		MenuController->ShowSettingsMenu();
	}
	
}

void AMenuBroomPawn::LoadLevel()
{
	if(!LevelToLoad.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(LevelToLoad));
	}
}

EButtonType AMenuBroomPawn::GetButtonType()
{
	return ButtonType;
}

void AMenuBroomPawn::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
	{
		if(TObjectPtr<UMenuActorUserWidget> MenuWidget = Cast<UMenuActorUserWidget>(MenuButtonWidget->GetWidget()))
		{
			const FSlateColor col = FSlateColor(FLinearColor::Red);
			MenuWidget->ButtonText->SetColorAndOpacity(col);
		}
	}
}

void AMenuBroomPawn::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
	{
		if(TObjectPtr<UMenuActorUserWidget> MenuWidget = Cast<UMenuActorUserWidget>(MenuButtonWidget->GetWidget()))
		{
			MenuWidget->ButtonText->SetColorAndOpacity(StartSlateColor);
		}
	}
}

void AMenuBroomPawn::Move(const FInputActionValue& Value)
{
	float Movement = Value.Get<float>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();

		// get forward vector
		const FVector UpDirection = FVector::ZAxisVector;

		// add movement
		FVector Translation = UpDirection * Movement;

		// Need to do a check to make sure that the player is inside the screenspace
		float Speed = 10.0f;
		AddActorWorldOffset(Translation * Speed);
	}
}

