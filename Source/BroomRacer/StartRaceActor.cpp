// Fill out your copyright notice in the Description page of Project Settings.


#include "StartRaceActor.h"



// Sets default values
AStartRaceActor::AStartRaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(RootComp);


}

// Called when the game starts or when spawned
void AStartRaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartRaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

