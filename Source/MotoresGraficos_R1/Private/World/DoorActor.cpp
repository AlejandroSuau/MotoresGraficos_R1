#include "World/DoorActor.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(SceneRoot);

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetupAttachment(SceneRoot);
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	ClosedRotation = DoorMesh->GetRelativeRotation();
	TargetRotation = ClosedRotation;
	
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(
		this,
		&ADoorActor::OnInteractionBegin);

	InteractionTrigger->OnComponentEndOverlap.AddDynamic(
		this,
		&ADoorActor::OnInteractionEnd);
}

void ADoorActor::OnInteractionBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		bPlayerNearby = true;
		Player->SetCurrentDoor(this);
	}
}

void ADoorActor::OnInteractionEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		bPlayerNearby = false;
		Player->SetCurrentDoor(nullptr);
	}
}

void ADoorActor::Interact()
{
	if (!bPlayerNearby) { return; }

	bIsOpen = !bIsOpen;
	TargetRotation = ClosedRotation;

	if (bIsOpen)
	{
		TargetRotation.Yaw += OpenAngle;
	}
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator NewRotation = FMath::RInterpTo(
		DoorMesh->GetRelativeRotation(),
		TargetRotation,
		DeltaTime,
		OpenSpeed
	);

	DoorMesh->SetRelativeRotation(NewRotation);
}