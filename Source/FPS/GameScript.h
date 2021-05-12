// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameScript.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AGameScript : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName levelName;

	int enemyCount;

private:
	void ContinueGame();

public:
	void OnEnemySpawned();
	void OnEnemyDestroyed();
};
