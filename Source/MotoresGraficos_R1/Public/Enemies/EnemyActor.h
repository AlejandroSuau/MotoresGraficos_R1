#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class MOTORESGRAFICOS_R1_API AEnemyActor : public AActor
{
	GENERATED_BODY()

public:
	AEnemyActor();

	void ReceiveDamage(float DamageAmount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float Health = 100.0f;
};