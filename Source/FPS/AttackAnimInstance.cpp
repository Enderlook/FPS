// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimInstance.h"

void UAttackAnimInstance::SetAttackCallback(IHasAttack* callback)
{
	hasAttack = callback;
}

void UAttackAnimInstance::TryAttack()
{
	if (isAttacking)
		return;
	isAttacking = true;
}

void UAttackAnimInstance::OnAttack()
{
	if (hasAttack)
		hasAttack->OnAttack();
}

void UAttackAnimInstance::OnEndAttack()
{
	isAttacking = false;
	if (hasAttack)
		hasATtack->OnEndAttack();
}