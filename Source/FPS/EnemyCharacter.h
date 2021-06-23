// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Damagable.h"
#include "EnemyAIController.h"
#include "Internationalization/Text.h"
#include "GameScript.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FPS_API AEnemyCharacter : public ACharacter, public IDamagable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Character")
	int hitpoints;

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

	bool isAttacking;

	UTextRenderComponent* textComponent;

	UCharacterMovementComponent* movementComponent;

	float initialMaxWalkSpeed;
	float initialMaxWalkSpeedCrouched;
	float initialMaxAcceleration;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

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
	virtual void AttackStart();
	void AttackCallback();
	class AEnemyAIController* GetAIController();
	class AGameScript* GetGameMode();
	bool IsPlayerInSight();
	FVector GetLastKnownPlayerLocation();
	bool IsAlive();

private:
	int GetWaypointIndex();
	void SetMovementSpeed(bool isChasingPlayer);
};
