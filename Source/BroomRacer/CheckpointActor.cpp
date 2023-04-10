// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointActor.h"

#include "PlayerBroomPawn.h"
#include "Components/CapsuleComponent.h"

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
}

// Called when the game starts or when spawned
void ACheckpointActor::BeginPlay()
{
	Super::BeginPlay();
	
	CheckpointCollision->OnComponentEndOverlap.AddDynamic(this, &ACheckpointActor::OnEndOverlap);
	
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

	if(!bIsFound && OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		// Stops calling for each component on other actor
		// Ideally would only check for the capsule but that gets called multiple times,
		// whereas the mesh gets called once
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			APlayerBroomPawn* Actor = Cast<APlayerBroomPawn>(OtherActor);
			Actor->CheckpointsPassed++;
			ActorsPassedThrough.Add(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Passed"));
		}
	}
}

