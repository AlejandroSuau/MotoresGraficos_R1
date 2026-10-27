#include "Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "World/DoorActor.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Enemies/EnemyActor.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Store horizontal speed so it can be used by the Animation Blueprint.
	Speed = GetVelocity().Size2D();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add the player's input mapping context to the Enhanced Input subsystem.
	if (auto* PlayerController = Cast<APlayerController>(Controller))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) { return; }

	// Movement
	EnhancedInputComponent->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::Move);

	// Camera
	EnhancedInputComponent->BindAction(
		LookAction,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::Look);

	// Jump
	EnhancedInputComponent->BindAction(
		JumpAction,
		ETriggerEvent::Started,
		this,
		&APlayerCharacter::Jump);

	EnhancedInputComponent->BindAction(
		JumpAction,
		ETriggerEvent::Completed,
		this,
		&APlayerCharacter::StopJumping);

	// Shooting
	EnhancedInputComponent->BindAction(
		ShootAction,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::StartShooting);

	EnhancedInputComponent->BindAction(
		ShootAction,
		ETriggerEvent::Completed,
		this,
		&APlayerCharacter::StopShooting);

	EnhancedInputComponent->BindAction(
		InteractAction,
		ETriggerEvent::Started,
		this,
		&APlayerCharacter::Interact);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) { return; }

	// Only use the camera's Yaw so movement remains parallel to the ground.
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::StartShooting()
{
	if (!bCanShoot) { return; }

	bCanShoot = false;

	GetWorldTimerManager().SetTimer(
		FireRateTimer,
		this,
		&APlayerCharacter::ResetFireRate,
		FireRate,
		false);
	
	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (ShootMontage
		&& AnimInstance
		&& !AnimInstance->Montage_IsPlaying(ShootMontage))
	{
		AnimInstance->Montage_Play(ShootMontage);
	}

	auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) { return; }

	// First trace: find what the player is aiming at from the camera
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Rotate the character towards the camera direction
	FRotator NewCharacterRotation = GetActorRotation();
	NewCharacterRotation.Yaw = CameraRotation.Yaw;

	SetActorRotation(NewCharacterRotation);

	const float TraceDistance = 5000.0f;
	const FVector CameraTraceEnd = CameraLocation + CameraRotation.Vector() * TraceDistance;

	FHitResult CameraHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		CameraHit,
		CameraLocation,
		CameraTraceEnd,
		ECC_Visibility,
		QueryParams);

	const FVector AimPoint = CameraHit.bBlockingHit ? CameraHit.ImpactPoint : CameraTraceEnd;

	// Find the rifle component.
	auto WeaponComponents = GetComponentsByTag(
			UStaticMeshComponent::StaticClass(),
			FName("Weapon"));

	if (WeaponComponents.IsEmpty()) { return; }

	auto* WeaponMesh = Cast<UStaticMeshComponent>(WeaponComponents[0]);
	if (!WeaponMesh) { return; }

	const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("MuzzleSocket"));

	// Second trace: the actual shot starts at the rifle muzzle
	// and travels towards the point selected by the camera.
	const FVector ShotDirection = (AimPoint - MuzzleLocation).GetSafeNormal();
	const FVector ShotEnd = MuzzleLocation + ShotDirection * TraceDistance;

	FHitResult ShotHit;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		ShotHit,
		MuzzleLocation,
		ShotEnd,
		ECC_Visibility,
		QueryParams);

	DrawDebugLine(
		GetWorld(),
		MuzzleLocation,
		bHit ? ShotHit.ImpactPoint : ShotEnd,
		FColor::Red,
		false,
		1.0f,
		0,
		1.0f);

	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			ShotHit.ImpactPoint,
			10.0f,
			12,
			FColor::Green,
			false,
			1.0f);

		if (auto* HitActor = ShotHit.GetActor())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Hit: %s"),
				*HitActor->GetName());
		}

		if (auto* Enemy = Cast<AEnemyActor>(ShotHit.GetActor()))
		{
			Enemy->ReceiveDamage(WeaponDamage);
		}
	}
}

void APlayerCharacter::ResetFireRate()
{
	bCanShoot = true;
}

void APlayerCharacter::StopShooting()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (ShootMontage && AnimInstance)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, ShootMontage);
	}
}

void APlayerCharacter::SetCurrentDoor(ADoorActor* Door)
{
	CurrentDoor = Door;
}

void APlayerCharacter::Interact()
{
	if (CurrentDoor)
	{
		CurrentDoor->Interact();
	}
}