// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemyCharacter.h"

void AShooterEnemyCharacter::AttackStart()
{
	Super::AttackStart();

	AEnemyAIController* controller = GetAIController();
	if (controller)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AShooterEnemyCharacter::Attack, fireWarmup, false, fireWarmup);
	}
}

void AShooterEnemyCharacter::Attack()
{
	if (bulletClass)
	{
		FVector eyesLocation;
		FRotator eyesRotation;
		GetActorEyesViewPoint(eyesLocation, eyesRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		FVector muzzleOffset = FVector(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector position = eyesLocation + FTransform(eyesRotation).TransformVector(muzzleOffset);

		FVector direction = GetLastKnownPlayerLocation() - eyesLocation;
		direction.Normalize();

		ABullet::SpawnAndShoot(this, bulletClass, position, direction.Rotation());
	}

	AttackCallback();
}