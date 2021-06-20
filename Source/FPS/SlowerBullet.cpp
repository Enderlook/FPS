// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowerBullet.h"

void ASlowerBullet::OnHit(AActor* OtherActor)
{
	ATPS_Player* player = Cast<ATPS_Player>(OtherActor);
	if (player)
		player->ModifySpeed(0.66f, 2.5f);
}