// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineTrackCreatorActor.h"

#include "CheckpointActor.h"
#include "Components/SplineComponent.h"

// Sets default values
ASplineTrackCreatorActor::ASplineTrackCreatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASplineTrackCreatorActor::BeginPlay()
{
	Super::BeginPlay();

	if(!UseConstructionScript)
		CreateTrack();
}

void ASplineTrackCreatorActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(UseConstructionScript)
		CreateTrack();
	else
		ClearTrack();
}

// Called every frame
void ASplineTrackCreatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineTrackCreatorActor::CreateTrack()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	if(StartActor == nullptr)
	{
		FTransform Trans;
		Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
		Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World).Quaternion());
		
		StartActor = GetWorld()->SpawnActor<AActor>(StartActorClass, Trans);
	}
	else
	{
		StartActor->SetActorLocation(SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
		StartActor->SetActorRotation(SplineComponent->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World));
		
	}

	
	for(int i = 1; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
	{
		FTransform Trans;
		Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World).Quaternion());
		
		if(CheckpointActors.Find(i) == nullptr)
		{
			ACheckpointActor* Actor = GetWorld()->SpawnActor<ACheckpointActor>(CheckpointClass, Trans, Params);
			Actor->CheckpointIndex = i;
			CheckpointActors.Add(i, Actor);
		}
		else
		{
			if(*CheckpointActors.Find(i) != nullptr)
			{
				auto Actor = *CheckpointActors.Find(i);
				Actor->SetActorLocation(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
				Actor->SetActorRotation(SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World));
				Actor->CheckpointIndex = i;
				
			}
			else // if mesh no longer exists tell the map to clear so it can spawn a new one
			{
				CheckpointActors.FindAndRemoveChecked(i);
			}
		}
	}

	if(EndActor == nullptr)
	{
		FTransform Trans;
		Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
		Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World).Quaternion());
		EndActor = GetWorld()->SpawnActor<AActor>(EndActorClass, Trans);
		
	}
	else
	{
		EndActor->SetActorLocation(SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
		EndActor->SetActorRotation(SplineComponent->GetRotationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
	}
}

void ASplineTrackCreatorActor::ClearTrack()
{
	if(!CheckpointActors.IsEmpty())
	{
		for(auto& Data : CheckpointActors)
		{
			if(Data.Value != nullptr)
				Data.Value->Destroy();
		}
		CheckpointActors.Empty();
	}

	if(StartActor != nullptr)
	{
		StartActor->Destroy();
		StartActor = nullptr;
	}

	if(EndActor != nullptr)
	{
		EndActor->Destroy();
		EndActor = nullptr;
	}
}


