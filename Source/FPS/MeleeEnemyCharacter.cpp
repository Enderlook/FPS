// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> impactSoundResource(TEXT("/Game/Audio/Sounds/Melee_Impact_Cue.Melee_Impact_Cue"));
	if (impactSoundResource.Succeeded())
		impactSound = impactSoundResource.Object;
}

void AMeleeEnemyCharacter::AttackStart()
{
	Super::AttackStart();

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &AMeleeEnemyCharacter::Attack, 1, false, 1);
}

void AMeleeEnemyCharacter::Attack()
{
	if (!IsAlive())
		return;

	TArray<AActor*> actorsToIgnore = TArray<AActor*>();
	actorsToIgnore.Add(this);
	TArray<AActor*> actors = TArray<AActor*>();

	if (UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(),
		GetActorLocation() + FVector(100.0f, 0.0f, 0.0f),
		75.0f, 150.0f,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		nullptr,
		actorsToIgnore,
		actors))
	{
		for (AActor* actor : actors)
		{
			if (actor == this)
				continue;

			IDamagable* damagable = Cast<IDamagable>(actor);
			if (damagable != nullptr)
				damagable->TakeDamage();
		}

		if (impactSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), impactSound, GetActorLocation());
	}
	AttackCallback();
}