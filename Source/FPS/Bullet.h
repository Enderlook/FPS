// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Bullet.generated.h"

UCLASS()
class FPS_API ABullet : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* bulletMovementComponent;

	const AActor* ownerActor;

	USoundCue* impactSound;

public:	
	// Sets default values for this actor's properties
	ABullet();

private:
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void OnHit(AActor* OtherActor);

public:
	static ABullet* SpawnAndShoot(AActor* actor, TSubclassOf<class ABullet> bulletClass, FVector position, FRotator rotation, USoundCue* shootSound);
};
