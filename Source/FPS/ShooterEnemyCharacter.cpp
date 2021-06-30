// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemyCharacter.h"

AShooterEnemyCharacter::AShooterEnemyCharacter()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> shootSoundHelper(TEXT("/Game/Audio/Sounds/Shoot/Shoot_Cue.Shoot_Cue"));
	if (shootSoundHelper.Succeeded())
		shootSound = shootSoundHelper.Object;
}

void AShooterEnemyCharacter::AttackLogic()
{
	if (!IsAlive())
		return;

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

		ABullet::SpawnAndShoot(this, bulletClass, position, direction.Rotation(), shootSound);
	}
}