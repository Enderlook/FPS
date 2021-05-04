// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"

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

	CheckPlayerIsInSight();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::MoveToNextWaypoint()
{
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

	if (controller)
	{
		int newWaypoint = FMath::RandRange(0, waypoints.Num() - 1);
		AActor* actor = waypoints[newWaypoint];
		controller->MoveToActor(actor, waypointAceptanceRadius, false);
	}
}

void AEnemyCharacter::MoveToPlayer()
{
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

	if (controller)
		controller->MoveToActor(player, playerAceptanceRadius, false);
}

void AEnemyCharacter::MoveToLastPlayerKnownLocation()
{
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

	if (controller)
		controller->MoveToLocation(lastKnownPlayerPosition, playerAceptanceRadius, false);
}

void AEnemyCharacter::CheckPlayerIsInSight()
{
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

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