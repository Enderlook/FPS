// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyCharacter.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	collisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Attack"));
	collisionComponent->InitCapsuleSize(75.0f, 150.0f);
	collisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Melee"));
	collisionComponent->SetupAttachment(RootComponent);
	collisionComponent->AddLocalOffset(FVector(100.0f, 0.0f, 0.0f));
}

void AMeleeEnemyCharacter::AttackStart()
{
	Super::AttackStart();

	AEnemyAIController* controller = GetAIController();
	if (controller)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AMeleeEnemyCharacter::Attack, 1, false, 1);
	}
}

void AMeleeEnemyCharacter::Attack()
{
	TArray<AActor*> actors = TArray<AActor*>();
	collisionComponent->GetOverlappingActors(actors);
	for (AActor* actor : actors)
	{
		if (actor == this)
			continue;

		IDamagable* damagable = Cast<IDamagable>(actor);
		if (damagable != nullptr)
			damagable->TakeDamage();
	}
	AttackCallback();
}