#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class MOTORESGRAFICOS_R1_API APortalActor : public AActor
{
	GENERATED_BODY()

public:
	APortalActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RingLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RingNebulous;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TeleportTrigger;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Portal")
	APortalActor* LinkedPortal = nullptr;

	UFUNCTION()
	void OnPortalOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};