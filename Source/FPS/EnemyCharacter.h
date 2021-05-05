// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FPS_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float waypointAceptanceRadius;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> waypoints;
	int waypointIndex;

	AActor* player;
	FVector lastKnownPlayerPosition;

	UPROPERTY(EditAnywhere)
	float playerAceptanceRadius;

	UPROPERTY(EditAnywhere)
	float sightRadius;

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

	void MoveToCurrentWaypoint();

	void MoveToNextWaypoint();

	void MoveToPlayer();

	void MoveToLastPlayerKnownLocation();

	int GetWaypointIndex();

private:
	void CheckPlayerIsInSight();
};
