// Fill out your copyright notice in the Description page of Project Settings.


#include "StartRaceActor.h"

#include "PlayerBroomPawn.h"
#include "Components/BoxComponent.h"


// Sets default values
AStartRaceActor::AStartRaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	if(OtherActor->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		if(OtherComp->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			APlayerBroomPawn* Actor = Cast<APlayerBroomPawn>(OtherActor);
			Actor->StartLapTime();
		}
	}
}

