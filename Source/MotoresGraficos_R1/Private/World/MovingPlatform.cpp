#include "World/MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	SetRootComponent(PlatformMesh);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(PointA);
}


void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Wait at Point A or Point B before moving again.
	if (bIsWaiting)
	{
		WaitTimer += DeltaTime;
		if (WaitTimer >= WaitTime)
		{
			WaitTimer = 0.0f;
			bIsWaiting = false;
			bMovingToPointB = !bMovingToPointB;
		}

		return;
	}

	const auto CurrentLocation = GetActorLocation();
	const auto TargetLocation = bMovingToPointB ? PointB : PointA;
	const auto  Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	const float DistanceToTarget = FVector::Distance(CurrentLocation, TargetLocation);

	// Estimate the distance required to stop with the current acceleration.
	const float BrakingDistance = (CurrentSpeed * CurrentSpeed) / (2.0f * Acceleration);
	if (DistanceToTarget <= BrakingDistance)
	{
		// Decelerate when approaching the destination.
		CurrentSpeed = FMath::Max(0.0f, CurrentSpeed - Acceleration * DeltaTime);
	}
	else
	{
		// Accelerate until reaching maximum speed.
		CurrentSpeed = FMath::Min(MaxSpeed,CurrentSpeed + Acceleration * DeltaTime);
	}

	const auto NewLocation = CurrentLocation + Direction * CurrentSpeed * DeltaTime;

	// Prevent overshooting the destination.
	if (FVector::DistSquared(NewLocation, TargetLocation) <= FMath::Square(CurrentSpeed * DeltaTime))
	{
		SetActorLocation(TargetLocation);

		CurrentSpeed = 0.0f;
		bIsWaiting = true;
		WaitTimer = 0.0f;

		return;
	}

	SetActorLocation(NewLocation);
}

