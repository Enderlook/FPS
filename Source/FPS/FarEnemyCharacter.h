// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "FarEnemyCharacter.generated.h"

UCLASS()
class FPS_API AFarEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Enemy Character|Attack")
	float minPlayerDistance;

protected:
	virtual bool CanAttack() override;
};
