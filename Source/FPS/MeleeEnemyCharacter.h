// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AMeleeEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void AttackStart() override;

private:
	void Attack();
};
