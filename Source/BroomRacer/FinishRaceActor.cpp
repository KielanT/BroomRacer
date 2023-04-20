// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishRaceActor.h"

#include "BroomRacerGameMode.h"
#include "CheckpointActor.h"
#include "PlayerBroomPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFinishRaceActor::AFinishRaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComp);

	// Sets collision boxes
	BlockLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Left"));
	BlockLeft->SetupAttachment(MeshComponent);

	BlockRight = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Right"));
	BlockRight->SetupAttachment(MeshComponent);
	
	BlockTop = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Top"));
	BlockTop->SetupAttachment(MeshComponent);
	
	BlockBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Bottom"));
	BlockBottom->SetupAttachment(MeshComponent);

	// Finish collision box
	FinishCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Col"));
	FinishCollisionBox->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AFinishRaceActor::BeginPlay()
{
	Super::BeginPlay();
	
	// binds the collision box
	FinishCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AFinishRaceActor::OnEndOverlap);
	
	GameModeRef = Cast<ABroomRacerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Get all the checkpoints
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointActor::StaticClass(), CheckpointActors);
	
}

// Called every frame
void AFinishRaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishRaceActor::OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlaped actor is the broom
	if(OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		// An extra check to make sure that this is only called once
		// since there are multiple components that can be used to collide
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			APlayerBroomPawn* Actor = Cast<APlayerBroomPawn>(OtherActor);
			Actor->StopLapTime(); // Stops the lap time

			for(auto checkpointActor : CheckpointActors)
			{
				if(ACheckpointActor* Checkpoint = Cast<ACheckpointActor>(checkpointActor))
				{
					Checkpoint->ActorsPassedThrough.Empty(); // resets the checkpoints 
				}
			}

			GameModeRef->RaceFinished(); // Sets the race/lap to be finished
		}
	}
	
	
}
