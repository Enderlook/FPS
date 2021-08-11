// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Area.h"
#include "FarEnemyCharacter.h"
#include "Sound/SoundCue.h"
#include "AreaEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AAreaEnemyCharacter : public AFarEnemyCharacter
{
	GENERATED_BODY()

public:
	AAreaEnemyCharacter();

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character|Attack")
	TSubclassOf<class AArea> areaClass;

	USoundCue* shootSound;

protected:
	virtual void AttackLogic() override;
};
