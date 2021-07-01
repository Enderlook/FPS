// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackAnimInstance.h"
#include "Components/TextRenderComponent.h"
#include "Damagable.h"
#include "Engine.h"
#include "Engine/World.h"
#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameScript.h"
#include "HasAttack.h"
#include "Internationalization/Text.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FPS_API AEnemyCharacter : public ACharacter, public IDamagable, public IHasAttack
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Character|Health")
	int hitpoints;
	
	USoundCue* deathSound;

	UPROPERTY(EditAnywhere, Category = "Enemy Character|NavAgent")
	float waypointAceptanceRadius;

	UPROPERTY(EditAnywhere, Category = "Enemy Character|NavAgent")
	TArray<AActor*> waypoints;
	int waypointIndex;

	AActor* player;
	FVector lastKnownPlayerPosition;

	UPROPERTY(EditAnywhere, Category = "Enemy Character|NavAgent")
	float playerAceptanceRadius;

	UPROPERTY(EditAnywhere, Category = "Enemy Character|NavAgent")
	float sightRadius;

	UPROPERTY(EditAnywhere, Category = "Enemy Character|NavAgent")
	float sightMaxAngle;

	UPROPERTY(EditAnywhere, Category = "Enemy Character|NavAgent")
	float speedIncreaseMultiplierWhenChasingPlayer;
		
	UPROPERTY(EditAnywhere, Category = "Enemy Character|Attack")
	UAttackAnimInstance* attackAnimation;

	UTextRenderComponent* textComponent;

	UCharacterMovementComponent* movementComponent;

	float initialMaxWalkSpeed;
	float initialMaxWalkSpeedCrouched;
	float initialMaxAcceleration;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(BlueprintReadWrite)
	bool isAttacking;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage() override;
	void MoveToCurrentWaypoint();
	void MoveToNextWaypoint();
	void MoveToPlayer();
	void MoveToLastPlayerKnownLocation();
	void Attack();
	void SetText(FText text);

protected:
	virtual void AttackLogic();
	void AttackLogicFallback();
	virtual void OnAttack() override;
	virtual void OnEndAttack() override;
	class AEnemyAIController* GetAIController();
	class AGameScript* GetGameMode();
	bool IsPlayerInSight();
	FVector GetLastKnownPlayerLocation();
	bool IsAlive();

private:
	int GetWaypointIndex();
	void SetMovementSpeed(bool isChasingPlayer);
};
