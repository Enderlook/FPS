// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimInstance.h"

void UAttackAnimInstance::SetAttackCallback(IHasAttack* callback)
{
	hasAttack = callback;
}

void UAttackAnimInstance::OnAttack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("On Attack"));

	if (hasAttack)
		hasAttack->OnAttack();
}

void UAttackAnimInstance::OnEndAttack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("On End Attack"));

	if (hasAttack)
		hasAttack->OnEndAttack();
}