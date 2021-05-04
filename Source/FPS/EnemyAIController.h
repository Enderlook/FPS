// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

enum class EState : uint8
{
	ES_Patrol,
	ES_Hunt,
	ES_Chase,
};

/**
 * 
 */
UCLASS()
class FPS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	EState state;

private:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	void GoToHuntState();
	void GoToChaseState();
	void GoToPatrolState();

public:
	void CanSeePlayer(bool can);
};
