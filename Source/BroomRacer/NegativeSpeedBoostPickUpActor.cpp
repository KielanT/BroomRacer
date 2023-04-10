// Fill out your copyright notice in the Description page of Project Settings.


#include "NegativeSpeedBoostPickUpActor.h"

#include "PlayerBroomPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"

void ANegativeSpeedBoostPickUpActor::OnPickUp(APawn* Pawn)
{
	Super::OnPickUp(Pawn);
	if(Pawn->GetClass()->IsChildOf(APlayerBroomPawn::StaticClass()))
	{
		BroomPawn = Cast<APlayerBroomPawn>(Pawn);
		PreviousSpeed = BroomPawn->DefaultSpeed;
		BroomPawn->GetMovement()->MaxSpeed /= SpeedMultiplier;
		UE_LOG(LogTemp, Warning, TEXT("max speed = %f"), BroomPawn->GetMovement()->MaxSpeed);
		UE_LOG(LogTemp, Warning, TEXT("max cal speed = %f"), BroomPawn->GetMovement()->MaxSpeed /= SpeedMultiplier);
		GetWorld()->GetTimerManager().SetTimer(SpeedBoostTimer, this, &ANegativeSpeedBoostPickUpActor::OnTimerFinished, SpeedBoostRate, false);
		
		// Hides and disables the pickup
		MeshComponent->SetVisibility(false);
		PickUpCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ANegativeSpeedBoostPickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ANegativeSpeedBoostPickUpActor::OnTimerFinished()
{
	BroomPawn->GetMovement()->MaxSpeed = PreviousSpeed;
	Destroy();
}
