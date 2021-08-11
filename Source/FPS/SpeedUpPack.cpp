// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedUpPack.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpeedUpPack::ASpeedUpPack()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> pickupSoundHelper(TEXT("/Game/Audio/Sounds/Speed_Up_Pack_Cue.Speed_Up_Pack_Cue"));
	if (pickupSoundHelper.Succeeded())
		pickupSound = pickupSoundHelper.Object;
}

// Called when the game starts or when spawned
void ASpeedUpPack::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* component = FindComponentByClass<UPrimitiveComponent>();
	if (component)
		component->OnComponentBeginOverlap.AddDynamic(this, &ASpeedUpPack::OnBeginOverlap);
}

void ASpeedUpPack::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATPS_Player* player = Cast<ATPS_Player>(OtherActor);
	if (player)
	{
		player->ModifySpeed(1 + speedFactor, speedDuration);
		if (pickupSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), pickupSound, player->GetActorLocation(), player->GetActorRotation());
		Destroy();
	}
}
