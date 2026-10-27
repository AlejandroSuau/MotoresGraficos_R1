// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAnimMontage;
class ADoorActor;
class UUserWidget;

UCLASS()
class MOTORESGRAFICOS_R1_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	void SetCurrentDoor(ADoorActor* Door);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override; 

	// Handles character movement relative to the camera direction.
	void Move(const FInputActionValue& Value);

	// Handles camera rotation using mouse input.
	void Look(const FInputActionValue& Value);

	// Starts and stops the shooting animation montage.
	void StartShooting();
	void StopShooting();

	// Input mapping context used by the player.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShootAction { nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction { nullptr };
	
	// Animation montage played while shooting.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* ShootMontage { nullptr };

	// Horizontal movement speed used by the Animation Blueprint.
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed { 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate = 0.15f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget = nullptr;
	
	UPROPERTY()
	ADoorActor* CurrentDoor = nullptr;

	bool bCanShoot = true;
	FTimerHandle FireRateTimer;

	void Interact();
	void ResetFireRate();
};
