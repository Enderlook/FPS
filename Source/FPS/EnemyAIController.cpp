// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyCharacter.h"

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AEnemyCharacter* character = Cast<AEnemyCharacter>(GetPawn());

	if (character)
	{
		switch (state)
		{
		case EState::ES_Patrol:
			character->MoveToNextWaypoint();
			break;
		case EState::ES_Hunt:
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

void AEnemyAIController::GoToHuntState()
{
	state = EState::ES_Hunt;

	AEnemyCharacter* character = Cast<AEnemyCharacter>(GetPawn());

	if (character)
		character->MoveToPlayer();
}

void AEnemyAIController::GoToChaseState()
{
	state = EState::ES_Chase;

	AEnemyCharacter* character = Cast<AEnemyCharacter>(GetPawn());

	if (character)
		character->MoveToLastPlayerKnownLocation();
}

void AEnemyAIController::GoToPatrolState()
{
	state = EState::ES_Patrol;

	AEnemyCharacter* character = Cast<AEnemyCharacter>(GetPawn());

	if (character)
		character->MoveToNextWaypoint();
}