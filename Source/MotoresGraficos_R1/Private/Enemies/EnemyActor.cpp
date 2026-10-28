#include "Enemies/EnemyActor.h"

#include "Components/StaticMeshComponent.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	SetRootComponent(EnemyMesh);
}

void AEnemyActor::ReceiveDamage(float DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Destroy();
	}
}

