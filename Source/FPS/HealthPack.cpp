// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"

// Sets default values
AHealthPack::AHealthPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* component = FindComponentByClass<UPrimitiveComponent>();
	if (component)
		component->OnComponentBeginOverlap.AddDynamic(this, &AHealthPack::OnBeginOverlap);
}

void AHealthPack::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATPS_Player* player = Cast<ATPS_Player>(OtherActor);
	if (player)
	{
		player->RestoreHitpoints(restoredHitpoints);
		Destroy();
	}
}