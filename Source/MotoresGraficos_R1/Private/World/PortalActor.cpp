#include "World/PortalActor.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

APortalActor::APortalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(SceneRoot);

	TeleportTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportTrigger"));
	TeleportTrigger->SetupAttachment(SceneRoot);
}

void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	TeleportTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnPortalOverlap);
}

void APortalActor::OnPortalOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (auto* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (!Player->CanUsePortal() || !LinkedPortal) { return; }

		Player->StartPortalCooldown();
		
		if (LinkedPortal)
		{
			const float CapsuleHalfHeight = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			const FVector ExitLocation = (
				LinkedPortal->GetActorLocation()
				+ LinkedPortal->GetActorForwardVector() * 150.0f
				+ FVector(0.0f, 0.0f, CapsuleHalfHeight));
			const FRotator ExitRotation = LinkedPortal->GetActorForwardVector().Rotation();
			Player->SetActorLocationAndRotation(ExitLocation, ExitRotation);

			if (AController* PlayerController = Player->GetController())
			{
				PlayerController->SetControlRotation(ExitRotation);
			}
		}
	}
}