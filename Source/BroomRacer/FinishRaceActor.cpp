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

	BlockLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Left"));
	BlockLeft->SetupAttachment(MeshComponent);

	BlockRight = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Right"));
	BlockRight->SetupAttachment(MeshComponent);
	
	BlockTop = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Top"));
	BlockTop->SetupAttachment(MeshComponent);
	
	BlockBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Bottom"));
	BlockBottom->SetupAttachment(MeshComponent);

	FinishCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Col"));
	FinishCollisionBox->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AFinishRaceActor::BeginPlay()
{
	Super::BeginPlay();
	FinishCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AFinishRaceActor::OnEndOverlap);
	
	GameModeRef = Cast<ABroomRacerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	
	
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
	
	if(OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			APlayerBroomPawn* Actor = Cast<APlayerBroomPawn>(OtherActor);
			Actor->StopLapTime();

			for(auto checkpointActor : CheckpointActors)
			{
				if(ACheckpointActor* Checkpoint = Cast<ACheckpointActor>(checkpointActor))
				{
					Checkpoint->ActorsPassedThrough.Empty();
				}
			}

			GameModeRef->RaceFinished();
		}
	}
	
	
}
