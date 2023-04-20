// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBoostPickUpActor.h"
#include "Components/BoxComponent.h"
#include "PlayerBroomPawn.h"
#include "GameFramework/FloatingPawnMovement.h"

void ASpeedBoostPickUpActor::OnPickUp(APawn* Pawn)
{
	Super::OnPickUp(Pawn);

	if(Pawn->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		BroomPawn = Cast<APlayerBroomPawn>(Pawn);

		// Add the speed boost to the broom
		PreviousSpeed = BroomPawn->DefaultSpeed;
		PreviousAcceleration = BroomPawn->DefaultAcceleration;
		BroomPawn->GetMovement()->MaxSpeed *= SpeedMultiplier;
		BroomPawn->GetMovement()->Acceleration *= SpeedMultiplier;

		// Sets the timer 
		GetWorld()->GetTimerManager().SetTimer(SpeedBoostTimer, this, &ASpeedBoostPickUpActor::OnTimerFinished, SpeedBoostRate, false);
		
		// Hides and disables the pickup
		MeshComponent->SetVisibility(false);
		PickUpCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASpeedBoostPickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult); 
	// Just used the begin overlap function from the parent class
	// Will still call the correct pick up code
}

void ASpeedBoostPickUpActor::OnTimerFinished()
{
	// Resets the variables
	BroomPawn->GetMovement()->MaxSpeed = PreviousSpeed;
	BroomPawn->GetMovement()->Acceleration = PreviousAcceleration;
	
	// Destroys this pick up
	Destroy();
}
