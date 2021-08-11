// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaEnemyCharacter.h"

AAreaEnemyCharacter::AAreaEnemyCharacter()
{
	/*static ConstructorHelpers::FObjectFinder<USoundCue> shootSoundHelper(TEXT(""));
	if (shootSoundHelper.Succeeded())
		shootSound = shootSoundHelper.Object;*/
}

void AAreaEnemyCharacter::AttackLogic()
{
	if (!IsAlive())
		return;

	FVector playerPosition = GetLastKnownPlayerLocation();
	playerPosition.Z -= 90;

	FVector enemyPosition = GetActorLocation();
	enemyPosition.Z = playerPosition.Z;

	FVector direction = playerPosition - enemyPosition;
	direction.Normalize();

	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		FRotator rotation = direction.Rotation();

		world->SpawnActor<AArea>(areaClass, playerPosition, rotation, spawnParams);

		if (shootSound)
			UGameplayStatics::PlaySoundAtLocation(world, shootSound, playerPosition, rotation);
	}
}