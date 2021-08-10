// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "FarEnemyCharacter.h"
#include "Sound/SoundCue.h"
#include "ShooterEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AShooterEnemyCharacter : public AFarEnemyCharacter
{
	GENERATED_BODY()

public:
	AShooterEnemyCharacter();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character|Attack")
	TSubclassOf<class ABullet> bulletClass;

	USoundCue* shootSound;

protected:
	virtual void AttackLogic() override;
};
