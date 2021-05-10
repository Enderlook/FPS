// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AEnemyCharacter* character = GetCharacter();
	if (character)
	{
		switch (state)
		{
		case EState::ES_Patrol:
			character->MoveToNextWaypoint();
			break;
		case EState::ES_Hunt:
			character->Attack();
			break;
		case EState::ES_Chase:
			GoToPatrolState();
			break;
		}
	}
}

void AEnemyAIController::CanSeePlayer(bool can)
{
	switch (state)
	{
	case EState::ES_Patrol:
		if (can)
			GoToHuntState();
		break;
	case EState::ES_Hunt:
		if (!can)
			GoToChaseState();
		break;
	case EState::ES_Chase:
		if (can)
			GoToHuntState();
		break;
	}
}

void AEnemyAIController::FromAttack(bool canSeePlayer)
{
	if (canSeePlayer)
		GoToHuntState();
	else
		GoToPatrolState();
}

void AEnemyAIController::GoToHuntState()
{
	state = EState::ES_Hunt;

	AEnemyCharacter* character = GetCharacter();
	if (character)
		character->MoveToPlayer();
}

void AEnemyAIController::GoToChaseState()
{
	state = EState::ES_Chase;

	AEnemyCharacter* character = GetCharacter();
	if (character)
		character->MoveToLastPlayerKnownLocation();
}

void AEnemyAIController::GoToPatrolState()
{
	state = EState::ES_Patrol;

	AEnemyCharacter* character = GetCharacter();
	if (character)
		character->MoveToCurrentWaypoint();
}

void AEnemyAIController::SetDead()
{
	state = EState::ES_Dead;
}

void AEnemyAIController::OnBeingHurt()
{
	GoToChaseState();
}

AEnemyCharacter* AEnemyAIController::GetCharacter()
{
	return Cast<AEnemyCharacter>(GetPawn());
}