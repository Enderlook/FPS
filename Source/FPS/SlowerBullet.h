// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "TPS_Player.h"
#include "SlowerBullet.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ASlowerBullet : public ABullet
{
	GENERATED_BODY()

protected:
	virtual void OnHit(AActor* OtherActor) override;
};
