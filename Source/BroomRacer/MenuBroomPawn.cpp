// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBroomPawn.h"
#include "MenuButtonActorWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "MenuActorUserWidget.h"
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

}

void AMenuBroomPawn::Interact(APawn* InteractCharacter)
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepWorldTransform;
	InteractCharacter->AttachToActor(this, Rules);
	InteractCharacter->SetActorLocation(AttachLocation->GetComponentLocation());
	InteractCharacter->SetActorRotation(AttachLocation->GetComponentRotation());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
	MenuButtonWidget->SetVisibility(false);

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
}

void AMenuBroomPawn::LoadLevel()
{
	if(!LevelToLoad.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(LevelToLoad));
	}
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

