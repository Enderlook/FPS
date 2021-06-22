// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Sound/SoundCue.h"
#include "MeleeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AMeleeEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

private:
	USoundCue* impactSound;

protected:
	virtual void AttackStart() override;

public:
	AMeleeEnemyCharacter();

private:
	void Attack();
};
