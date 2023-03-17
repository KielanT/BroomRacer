// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuCameraActor.h"

#include "Camera/CameraComponent.h"

// Sets default values
AMenuCameraActor::AMenuCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(Camera);
}

// Called when the game starts or when spawned
void AMenuCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenuCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

