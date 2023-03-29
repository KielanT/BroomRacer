// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishRaceActor.h"

// Sets default values
AFinishRaceActor::AFinishRaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComp);
	
}

// Called when the game starts or when spawned
void AFinishRaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFinishRaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

