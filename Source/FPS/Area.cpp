// Fill out your copyright notice in the Description page of Project Settings.


#include "Area.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_Player.h"

// Sets default values
AArea::AArea()
{
	if (!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BoxSceneComponent"));

	areaMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMeshComponent"));
	areaMeshComponent->SetRelativeScale3D(FVector(20, 10, 10));
	areaMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("Area"));
	areaMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = areaMeshComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Meshes/aoe_hitbox.aoe_hitbox"));
	if (mesh.Succeeded())
		areaMeshComponent->SetStaticMesh(mesh.Object);

	/*static ConstructorHelpers::FObjectFinder<UMaterial> material(TEXT(""));
	if (material.Succeeded())
	{
		UMaterialInstanceDynamic* areaMaterialInstance = UMaterialInstanceDynamic::Create(material.Object, areaMeshComponent);
		areaMeshComponent->SetMaterial(0, areaMaterialInstance);

	static ConstructorHelpers::FObjectFinder<USoundCue> affectSoundHelper(TEXT(""));
	if (affectSoundHelper.Succeeded())
		affectSound = affectSoundHelper.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> unaffectSoundHelper(TEXT(""));
	if (unaffectSoundHelper.Succeeded())
		unaffectSound = unaffectSoundHelper.Object;*/
}

// Called when the game starts or when spawned
void AArea::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(warmupTime + duration);

	FTimerHandle handle;
	FTimerDelegate initializeDelegate = FTimerDelegate::CreateUObject(this, &AArea::Initialize);
	GetWorldTimerManager().SetTimer(handle, initializeDelegate, warmupTime, false);
}

void AArea::Initialize()
{
	areaMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnBeginOverlap);
	areaMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AArea::OnEndOverlap);

	ManualSetPlayer(true);

	FTimerHandle handle;
	FTimerDelegate deinitializeDelegate = FTimerDelegate::CreateUObject(this, &AArea::Deinitialize);
	GetWorldTimerManager().SetTimer(handle, deinitializeDelegate, duration, false);
}

void AArea::Deinitialize()
{
	if (hasPlayer)
		ManualSetPlayer(false);
}

void AArea::ManualSetPlayer(bool apply)
{
	TArray<AActor*> actors = TArray<AActor*>();
	areaMeshComponent->GetOverlappingActors(actors);
	for (AActor* actor : actors)
	{
		ATPS_Player* player = Cast<ATPS_Player>(actor);
		if (player)
		{
			player->ModifyFireRate(apply ? fireRateSlowdownFactor : -fireRateSlowdownFactor);
			hasPlayer = apply;
			break;
		}
	}
}

void AArea::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (hasPlayer)
		return;

	ATPS_Player* player = Cast<ATPS_Player>(OtherActor);

	if (player)
	{
		player->ModifyFireRate(fireRateSlowdownFactor);
		hasPlayer = true;

		if (affectSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), affectSound, SweepResult.ImpactPoint);
	}
}

void AArea::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!hasPlayer)
		return;

	ATPS_Player* player = Cast<ATPS_Player>(OtherActor);

	if (player)
	{
		player->ModifyFireRate(-fireRateSlowdownFactor);
		hasPlayer = false;

		if (unaffectSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), unaffectSound, player->GetActorLocation());
	}
}
