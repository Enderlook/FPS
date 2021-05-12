// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();

	textComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Status"));
	textComponent->SetupAttachment(RootComponent);
	textComponent->AddLocalOffset(FVector(0.0f, 0.0f, 90.0f));
	textComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	player = GetWorld()->GetFirstPlayerController()->GetPawn();

	AEnemyAIController* controller = GetAIController();
	if (controller)
		controller->Initialize();
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
		controller->MoveToLocation(GetLastKnownPlayerLocation(), playerAceptanceRadius, false);
}

bool AEnemyCharacter::IsPlayerInSight()
{
	// TODO: This could be replaced with PawnSensing component.

	AEnemyAIController* controller = GetAIController();
	if (player && controller)
	{
		if (player->GetDistanceTo(this) < sightRadius)
		{
			FVector playerPosition = player->GetActorLocation();
			FVector viewPoint;
			FRotator viewRotation;
			GetActorEyesViewPoint(viewPoint, viewRotation);
			FVector direction = (playerPosition - viewPoint);
			direction.Normalize();

			FVector forward = GetActorForwardVector();
			float angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forward, direction)));

			if (angle < sightMaxAngle)
			{
				FHitResult hit;
				FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
				// TODO: This could be replaced by `LineTraceSingleByObjectType` thought we would no longer be doing a manual raycast.
				ECollisionChannel defaultCollisionChannel = (ECollisionChannel)0;
				FPhysicsInterface::RaycastSingle(
					GetWorld(),
					OUT hit,
					viewPoint,
					playerPosition,
					defaultCollisionChannel,
					traceParams,
					FCollisionResponseParams::DefaultResponseParam,
					FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody)
				);

				AActor* actorHit = hit.GetActor();
				if (!actorHit || actorHit == player)
				{
					// We only perform the above raycast because it's requested by the exam,
					// actually the following check if much more accurate (it perfom several raycasts to check the size of the object),
					// that is why we also execute this after the first naive check.
					if (controller->LineOfSightTo(player, FVector::ZeroVector, false))
					{
						lastKnownPlayerPosition = player->GetActorLocation();
						return true;
					}
				}
			}
		}
	}
	return false;
}

void AEnemyCharacter::Attack()
{
	if (isAttacking)
		return;
	AttackStart();
}

void AEnemyCharacter::AttackStart()
{
	isAttacking = true;
}

void AEnemyCharacter::AttackCallback()
{
	isAttacking = false;
	AEnemyAIController* controller = GetAIController();
	if (controller)
		controller->FromAttack(IsPlayerInSight());
}

void AEnemyCharacter::TakeDamage()
{
	AEnemyAIController* controller = GetAIController();
	if (controller)
	{
		if (--hitpoints <= 0)
		{
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
}

AEnemyAIController* AEnemyCharacter::GetAIController()
{
	return Cast<AEnemyAIController>(GetController());
}

FVector AEnemyCharacter::GetLastKnownPlayerLocation()
{
	return lastKnownPlayerPosition;
}

void AEnemyCharacter::SetText(FText text)
{
	textComponent->SetText(text);
}