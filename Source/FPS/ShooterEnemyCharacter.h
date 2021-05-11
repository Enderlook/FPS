// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Bullet.h"
#include "ShooterEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AShooterEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character|Attack")
	TSubclassOf<class ABullet> bulletClass;

protected:
	virtual void AttackStart() override;

private:
	void Attack();
};
