// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Internationalization/Text.h"

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	GetWorldTimerManager().SetTimerForNextTick(this, &AEnemyAIController::NextMove);
}

void AEnemyAIController::NextMove()
{
	AEnemyCharacter* character = GetPawn<AEnemyCharacter>();
	if (character)
	{
		switch (state)
		{
		case EState::ES_Patrol:
			character->MoveToNextWaypoint();
			break;
		case EState::ES_Hunt:
			character->SetText(FText::FromString(FString("Attack")));
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

	AEnemyCharacter* character = GetPawn<AEnemyCharacter>();
	if (character)
	{
		character->SetText(FText::FromString(FString("Hunt")));
		character->MoveToPlayer();
	}
}

void AEnemyAIController::GoToChaseState()
{
	state = EState::ES_Chase;

	AEnemyCharacter* character = GetPawn<AEnemyCharacter>();
	if (character)
	{
		character->SetText(FText::FromString(FString("Chase")));
		character->MoveToLastPlayerKnownLocation();
	}
}

void AEnemyAIController::GoToPatrolState()
{
	state = EState::ES_Patrol;

	AEnemyCharacter* character = GetPawn<AEnemyCharacter>();
	if (character)
	{
		character->SetText(FText::FromString(FString("Patrol")));
		character->MoveToCurrentWaypoint();
	}
}

void AEnemyAIController::SetDead()
{
	state = EState::ES_Dead;
	AEnemyCharacter* character = GetPawn<AEnemyCharacter>();
	if (character)
		character->SetText(FText::FromString(FString("Death")));
}

void AEnemyAIController::OnBeingHurt()
{
	if (state == EState::ES_Dead)
		return;

	GoToChaseState();
}

void AEnemyAIController::Initialize()
{
	GoToPatrolState();
}