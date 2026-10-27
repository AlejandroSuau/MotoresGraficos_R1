#include "Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "World/DoorActor.h"

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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (ShootMontage
		&& AnimInstance
		&& !AnimInstance->Montage_IsPlaying(ShootMontage))
	{
		AnimInstance->Montage_Play(ShootMontage);
	}
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