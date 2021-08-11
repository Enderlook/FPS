// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "TPS_Player.h"
#include "SpeedUpPack.generated.h"

UCLASS()
class FPS_API ASpeedUpPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeedUpPack();

private:
	UPROPERTY(EditAnywhere)
	float speedFactor;
	
	UPROPERTY(EditAnywhere)
	float speedDuration;

	USoundCue* pickupSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
