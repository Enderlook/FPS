// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimInstance.h"

void UAttackAnimInstance::SetAttackCallback(IHasAttack* callback)
{
	hasAttack = callback;
}

void UAttackAnimInstance::OnAttack()
{
	if (hasAttack)
		hasAttack->OnAttack();
}

void UAttackAnimInstance::OnEndAttack()
{
	if (hasAttack)
		hasAttack->OnEndAttack();
}