// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();

	movementComponent = GetCharacterMovement();
	if (movementComponent)
	{
		initialMaxWalkSpeed = movementComponent->MaxWalkSpeed;
		initialMaxWalkSpeedCrouched = movementComponent->MaxWalkSpeedCrouched;
		initialMaxAcceleration = movementComponent->MaxAcceleration;
	}

	if (attackAnimation)
		attackAnimation = Cast<UAttackAnimInstance>(GetMesh()->GetAnimInstance());

	if (attackAnimation)
		attackAnimation->SetAttackCallback(this);

	static ConstructorHelpers::FObjectFinder<USoundCue> deathSoundHelper(TEXT("/Game/Audio/Sounds/Death/Death_Cue.Death_Cue"));
	if (deathSoundHelper.Succeeded())
		deathSound = deathSoundHelper.Object;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	player = GetWorld()->GetFirstPlayerController()->GetPawn();

	AEnemyAIController* controller = GetAIController();
	if (controller)
		controller->Initialize();

	AGameScript* gameScript = GetGameMode();
	if (gameScript)
		gameScript->OnEnemySpawned();
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
		SetMovementSpeed(false);
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
		SetMovementSpeed(false);
	}
}

int AEnemyCharacter::GetWaypointIndex()
{
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
	{
		controller->MoveToActor(player, playerAceptanceRadius, false);
		SetMovementSpeed(true);
	}
}

void AEnemyCharacter::MoveToLastPlayerKnownLocation()
{
	AEnemyAIController* controller = GetAIController();
	if (controller)
	{
		controller->MoveToLocation(GetLastKnownPlayerLocation(), playerAceptanceRadius, false);
		SetMovementSpeed(true);
	}
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

	isAttacking = true;
	if (!attackAnimation)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AEnemyCharacter::AttackLogicFallback, 1, false, 1);
	}
}

void AEnemyCharacter::AttackLogicFallback()
{
	OnAttack();
	OnEndAttack();
}

void AEnemyCharacter::AttackLogic()
{
}

void AEnemyCharacter::OnAttack()
{
	if (IsAlive())
		AttackLogic();
}

void AEnemyCharacter::OnEndAttack()
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
			if (hitpoints < 0)
				return;

			if (deathSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound, GetActorLocation(), GetActorRotation());

			controller->StopMovement();
			controller->SetDead();
			GetMesh()->SetSimulatePhysics(true);
			SetLifeSpan(3);

			AGameScript* gameScript = GetGameMode();
			if (gameScript)
				gameScript->OnEnemyDestroyed();
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
	return GetController<AEnemyAIController>();
}

FVector AEnemyCharacter::GetLastKnownPlayerLocation()
{
	return lastKnownPlayerPosition;
}

AGameScript* AEnemyCharacter::GetGameMode()
{
	return Cast<AGameScript>(GetWorld()->GetLevelScriptActor());
}

bool AEnemyCharacter::IsAlive()
{
	return hitpoints >= 0;
}

void AEnemyCharacter::SetMovementSpeed(bool isChasingPlayer)
{
	float multiplier;
	if (isChasingPlayer)
		multiplier = 1 + speedIncreaseMultiplierWhenChasingPlayer;
	else
		multiplier = 1;

	if (movementComponent)
	{
		movementComponent->MaxWalkSpeed = initialMaxWalkSpeed * multiplier;
		movementComponent->MaxWalkSpeedCrouched = initialMaxWalkSpeedCrouched * multiplier;
		movementComponent->MaxAcceleration = initialMaxAcceleration * multiplier;
	}
}

AActor* AEnemyCharacter::GetPlayer()
{
	return player;
}