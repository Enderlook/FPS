// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HasAttack.h"
#include "AttackAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UAttackAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	IHasAttack* hasAttack;

public:
	UPROPERTY(BlueprintReadOnly)
	bool isAttacking;

public:
	void SetAttackCallback(IHasAttack* callback);

	void TryAttack();

	UFUNCTION(BlueprintCallable)
	void OnAttack();

	UFUNCTION(BlueprintCallable)
	void OnEndAttack();
};
