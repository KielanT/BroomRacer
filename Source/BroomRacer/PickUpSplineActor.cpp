#include "PickUpSplineActor.h"
#include "BasePickUpActor.h"
#include "Components/SplineComponent.h"


// Sets default values
APickUpSplineActor::APickUpSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickUpSplineActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnPickUps();
}

// Called every frame
void APickUpSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APickUpSplineActor::SpawnPickUps()
{
	if(!Pickups.IsEmpty())
	{
		for(int i = 0; i <= SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
		{
			FTransform Trans;
			Trans.SetLocation(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
			Trans.SetRotation(SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World).Quaternion());

			int PickUpIndex = FMath::RandRange(1, Pickups.Num());
			auto PickUp = *Pickups.Find(PickUpIndex);
			GetWorld()->SpawnActor<ABasePickUpActor>(PickUp, Trans);
		}
	}
}

