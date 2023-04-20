// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineTrackCreatorActor.h"

#include "CheckpointActor.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

static TObjectPtr<ASplineTrackCreatorActor> Instance = nullptr;

// Sets default values
ASplineTrackCreatorActor::ASplineTrackCreatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);
	
	// Spline component used to create the track
	// Each spline point will have an actor on it (Start, checkpoint or end actor)
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASplineTrackCreatorActor::BeginPlay()
{
	Super::BeginPlay();

	// if the construction hasn't be used then create the track
	// On Begin Play
	if(!UseConstructionScript) 
		CreateTrack();

	// Make sure that the other maps are not pointing at the wrong spline
	Instance = nullptr;
}

void ASplineTrackCreatorActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// if the construction in use create the track
	// every time the track change it will update
	// Else clear the track delete all the actors
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

bool ASplineTrackCreatorActor::IsMulitpleLaps()
{
	return bIsMultipleLaps;
}

int ASplineTrackCreatorActor::GetLaps()
{
	return MaxLaps;
}

TObjectPtr<ASplineTrackCreatorActor> ASplineTrackCreatorActor::GetInstance()
{
	// If the instance is nullptr then find the track in world 
	// and set the instance pointer to it
	if(Instance == nullptr)
	{
		
		UWorld* WorldRef = GEngine->GameViewport->GetWorld();

		// GetActorOfClass returns the first actor of the this class 
		// This is only needed to be done once per level
		// if this was not done as a singleton then this function would be called a lot more
		Instance = Cast<ASplineTrackCreatorActor>(UGameplayStatics::GetActorOfClass(WorldRef, StaticClass()));
	}
	
	// If still null after trying to find the actor then return nullptr (less nested if statements and less checks)
	if(Instance == nullptr)
	{
		return nullptr;
	}

	
	
	return Instance;
}

void ASplineTrackCreatorActor::CreateTrack()
{

	FActorSpawnParameters Params;
	Params.Owner = this; // Sets the actors to be spawned to have this actor as it's owner

	if(StartActor == nullptr)
	{
		// Sets the start actor location and rotation at the first point 
		// on the spline
		
		FTransform Trans;
		Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
		Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World).Quaternion());
		
		// Spawns the actor and sets the pointer 
		StartActor = GetWorld()->SpawnActor<AActor>(StartActorClass, Trans);
	}
	else
	{
		// If not null then update it's position
		StartActor->SetActorLocation(SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
		StartActor->SetActorRotation(SplineComponent->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World));
	}

	// Loops through all the other spline points apart from the first and last
	for(int i = 1; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
	{
		// Sets the transform based on the spline points
		FTransform Trans;
		Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World).Quaternion());
		
		// If the checkpoint does not exist 
		// then create the checkpoint actor and add it to the checkpoint map
		if(CheckpointActors.Find(i) == nullptr)
		{
			ACheckpointActor* Actor = GetWorld()->SpawnActor<ACheckpointActor>(CheckpointClass, Trans, Params);
			Actor->CheckpointIndex = i;
			CheckpointActors.Add(i, Actor);
		}
		else
		{
			// if the checkpoint actor exists in the map then set the actors transform 
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

	// If the end actor does not exist then set the end actor to the last point on the spline
	if(EndActor == nullptr)
	{
		FTransform Trans;
		Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
		Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World).Quaternion());
		EndActor = GetWorld()->SpawnActor<AActor>(EndActorClass, Trans);
		
	}
	else
	{
		// If exists then update
		EndActor->SetActorLocation(SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
		EndActor->SetActorRotation(SplineComponent->GetRotationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
	}
}

void ASplineTrackCreatorActor::ClearTrack()
{


	if(!CheckpointActors.IsEmpty())
	{
		// If the checkpoint map has checkpoints
		// Destroy all the meshes
		// then empty the map
		
		for(auto& Data : CheckpointActors)
		{
			if(Data.Value != nullptr)
				Data.Value->Destroy();
		}
		CheckpointActors.Empty();
	}
	
	// Destroy the start actor if it exists
	if(StartActor != nullptr)
	{
		StartActor->Destroy();
		StartActor = nullptr;
	}

	// Destroy the end actor if it exists
	if(EndActor != nullptr)
	{
		EndActor->Destroy();
		EndActor = nullptr;
	}
}


