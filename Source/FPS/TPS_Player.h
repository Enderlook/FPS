// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackAnimInstance.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Bullet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Damagable.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "HasAttack.h"
#include "Sound/SoundCue.h"
#include "TPS_Player.generated.h"

UCLASS()
class FPS_API ATPS_Player : public ACharacter, public IDamagable, public IHasAttack
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPS_Player();

	UFUNCTION(BlueprintCallable)
	void Die();
	
	UPROPERTY(BlueprintReadOnly, Category = "Player|Health")
	int currentHitpoints;

private:
	UPROPERTY(EditAnywhere, Category = "Player|Health")
	int hitpoints;

	USoundCue* deathSound;

	UPROPERTY(EditAnywhere, Category = "Player|UI")
	TSubclassOf<UUserWidget> playerPowerWidgetClass;
	UUserWidget* playerPowerWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Attack")
	TSubclassOf<class ABullet> bulletClass;

	UPROPERTY(EditAnywhere, Category = "Player|Attack")
	UAttackAnimInstance* attackAnimation;

	USoundCue* shootSound;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	float runSpeedIncreaseMultiplier;

	bool isRunning;

	USkeletalMeshComponent* firingDrone;

	UCharacterMovementComponent* movementComponent;

	float initialMaxWalkSpeed;
	float initialMaxWalkSpeedCrouched;
	float initialMaxAcceleration;
	float speedMultiplier;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void TakeDamage() override;

	virtual void Jump() override;

	virtual void OnAttack() override;

	virtual void OnEndAttack() override;

	void RestoreHitpoints(int restoredHitpoints);

	void ModifySpeed(float factor, float duration);

private:
	UFUNCTION()
	void Fire();

	void MoveForward(float Axis);

	void MoveRight(float Axis);

	void RestartGame();

	UFUNCTION()
	void UnmodifySpeed(float factor);

	void StartRunning();

	void StopRunning();

	void SetMovementSpeed();
};
