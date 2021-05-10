// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveToNextWaypoint();

	player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEnemyAIController* controller = GetAIController();
	if (controller)
		controller->CanSeePlayer(IsPlayerInSight());
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::MoveToCurrentWaypoint()
{
	AEnemyAIController* controller = GetAIController();
	if (controller && waypoints.Num() > 0)
	{
		int index = GetWaypointIndex();
		AActor* actor = waypoints[index];
		controller->MoveToActor(actor, waypointAceptanceRadius, false);
	}
}

void AEnemyCharacter::MoveToNextWaypoint()
{
	AEnemyAIController* controller = GetAIController();
	if (controller && waypoints.Num() > 0)
	{
		waypointIndex++;
		int index = GetWaypointIndex();
		AActor* actor = waypoints[index];
		controller->MoveToActor(actor, waypointAceptanceRadius, false);
	}
}

int AEnemyCharacter::GetWaypointIndex() {
	int count = waypoints.Num() - 1;
	int max = count * 2;
	int index = waypointIndex % max;
	if (index >= count)
		return max - index;
	return index;
}

void AEnemyCharacter::MoveToPlayer()
{
	AEnemyAIController* controller = GetAIController();
	if (controller)
		controller->MoveToActor(player, playerAceptanceRadius, false);
}

void AEnemyCharacter::MoveToLastPlayerKnownLocation()
{
	AEnemyAIController* controller = GetAIController();
	if (controller)
		controller->MoveToLocation(lastKnownPlayerPosition, playerAceptanceRadius, false);
}

bool AEnemyCharacter::IsPlayerInSight()
{

	AEnemyAIController* controller = GetAIController();
	if (player && controller)
	{
		if (player->GetDistanceTo(this) < sightRadius)
		{
			lastKnownPlayerPosition = player->GetActorLocation();
			controller->CanSeePlayer(true);
		}
		else
		{
			controller->CanSeePlayer(false);
		}
	}
}

void AEnemyCharacter::TakeDamage()
{
	AEnemyAIController* controller = GetAIController();
	if (--hitpoints <= 0)
	{
		if (controller)
			controller->SetDead();

		GetMesh()->SetSimulatePhysics(true);
		SetLifeSpan(3);
	}
	else
	{
		lastKnownPlayerPosition = player->GetActorLocation();
		controller->OnBeingHurt();
	}
}

AEnemyAIController* AEnemyCharacter::GetAIController()
{
	return Cast<AEnemyAIController>(GetController());
}