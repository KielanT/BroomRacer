// Fill out your copyright notice in the Description page of Project Settings.


#include "StartRaceActor.h"

#include "PlayerBroomPawn.h"
#include "Components/BoxComponent.h"


// Sets default values
AStartRaceActor::AStartRaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(RootComp);

	StartCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Col"));
	StartCollisionBox->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AStartRaceActor::BeginPlay()
{
	Super::BeginPlay();

	// Bind the collision function end overlap
	StartCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AStartRaceActor::OnEndOverlap);
}

// Called every frame
void AStartRaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartRaceActor::OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// OnEndOverlap instead of on begin overlap so that it called once the player is completely through the start 

	// Check if the overlaping actor is the broom pawn
	if(OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{

		// The broom has multiple components that can be used for overlapping
		// Checking that the mesh if being overlapped means this functions only gets called once
		// A better way would be to create a collision channel 
		// TODO: Create a collision channel to remove the extra check
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			APlayerBroomPawn* Actor = Cast<APlayerBroomPawn>(OtherActor);
			Actor->StartLapTime();
		}
	}
}

