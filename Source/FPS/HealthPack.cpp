// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHealthPack::AHealthPack()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> pickupSoundHelper(TEXT("/Game/Audio/Sounds/Health_Pack_Cue.Health_Pack_Cue"));
	if (pickupSoundHelper.Succeeded())
		pickupSound = pickupSoundHelper.Object;
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
		if (pickupSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), pickupSound, player->GetActorLocation(), player->GetActorRotation());
		Destroy();
	}
}
