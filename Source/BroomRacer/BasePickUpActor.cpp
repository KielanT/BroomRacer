// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickUpActor.h"

#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ABasePickUpActor::ABasePickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// base pick up components, setsin the blueprints
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Up Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);

	// Pick up the collision box
	PickUpCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Pick Up Col box"));
	PickUpCollisionBox->SetupAttachment(MeshComponent);
	
}

// Called when the game starts or when spawned
void ABasePickUpActor::BeginPlay()
{
	Super::BeginPlay();

	// Binds the pickup collision function
	PickUpCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABasePickUpActor::OnBeginOverlap);
}

void ABasePickUpActor::OnPickUp(APawn* Pawn)
{
	// Implemented by the children
}

void ABasePickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->IsChildOf(APawn::StaticClass()))
	{
		APawn* Pawn = Cast<APawn>(OtherActor);
		OnPickUp(Pawn); // Calls the pickup function
	}
}

// Called every frame
void ABasePickUpActor::Tick(float DeltaTime)
{
	// Rotates the actor on the Y axis
	Super::Tick(DeltaTime);
	FRotator Rotator(0, 1.0f, 0);
	AddActorLocalRotation((Rotator * RotationSpeed) * DeltaTime);

}

