#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class MOTORESGRAFICOS_R1_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Movement")
	FVector PointA;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Movement")
	FVector PointB;

	// Maximum movement speed in cm/s.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxSpeed = 150.0f;

	// Controls how quickly the platform accelerates and decelerates.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Acceleration = 300.0f;

	// Time spent waiting at each endpoint.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WaitTime = 2.0f;

private:
	bool bMovingToPointB = true;
	bool bIsWaiting = false;

	float CurrentSpeed = 0.0f;
	float WaitTimer = 0.0f;
};