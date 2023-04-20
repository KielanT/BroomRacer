// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointActor.h"

#include "PlayerBroomPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ACheckpointActor::ACheckpointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(RootComp);

	CheckpointCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Checkpoint Collision"));
	CheckpointCollision->SetupAttachment(StaticMeshComponent);
	
	// Audio Component for playing sound when colliding
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->bAlwaysPlay = false;
}

// Called when the game starts or when spawned
void ACheckpointActor::BeginPlay()
{
	Super::BeginPlay();
	
	CheckpointCollision->OnComponentEndOverlap.AddDynamic(this, &ACheckpointActor::OnEndOverlap);
	CheckpointCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointActor::OnBeginOverlap);
	
}

// Called every frame
void ACheckpointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACheckpointActor::OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bool bIsFound = false;
	if(!ActorsPassedThrough.IsEmpty())
	{
		for(auto AlreadyPassedActor : ActorsPassedThrough)
		{
			if(AlreadyPassedActor == OtherActor)
			{
				bIsFound = true;
				break;
			}
		}
	}

	// TODO: Only Detect when certain part of the actor (the centre)
	
	if(!bIsFound && OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		// Stops calling for each component on other actor
		// Ideally would only check for the capsule or use a collision channel but that gets called multiple times,
		// whereas the mesh gets called once
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			APlayerBroomPawn* Actor = Cast<APlayerBroomPawn>(OtherActor);
			Actor->CheckpointsPassed++;
			ActorsPassedThrough.Add(OtherActor);
			
		}
	}
}

void ACheckpointActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// Checks if the checkpoint has already been passed through
	bool bIsFound = false;
	if(!ActorsPassedThrough.IsEmpty())
	{
		for(auto AlreadyPassedActor : ActorsPassedThrough)
		{
			if(AlreadyPassedActor == OtherActor)
			{
				bIsFound = true;
				break;
			}
		}
	}

	if (!bIsFound && OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		// Stops calling for each component on other actor
		// Ideally would only check for the capsule but that gets called multiple times,
		// whereas the mesh gets called once
		if (OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			AudioComponent->Play(); // Plays the sound but there is a slight delay
		}
	}
}

