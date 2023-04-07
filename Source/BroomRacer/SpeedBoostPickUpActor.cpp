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
		UE_LOG(LogTemp, Warning, TEXT("test picked up"));
		PreviousSpeed = BroomPawn->GetMovement()->MaxSpeed;
		BroomPawn->GetMovement()->MaxSpeed *= SpeedMultiplier;

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
}

void ASpeedBoostPickUpActor::OnTimerFinished()
{
	BroomPawn->GetMovement()->MaxSpeed = PreviousSpeed;
	Destroy();
}
