// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterEnemyCharacter.h"
#include "ShooterFarEnemyCharacter.generated.h"

UCLASS()
class FPS_API AShooterFarEnemyCharacter : public AShooterEnemyCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Enemy Character|Attack")
	float minPlayerDistance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
