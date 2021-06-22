// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Damagable.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 3.0f;

	if (!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	
	USphereComponent* collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(15.0f);
	collisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBeginOverlap);
	RootComponent = collisionComponent;

	bulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	bulletMovementComponent->SetUpdatedComponent(collisionComponent);
	bulletMovementComponent->InitialSpeed = 3000.0f;
	bulletMovementComponent->MaxSpeed = 3000.0f;
	bulletMovementComponent->bRotationFollowsVelocity = true;
	bulletMovementComponent->bShouldBounce = true;
	bulletMovementComponent->Bounciness = 0.3f;
	bulletMovementComponent->ProjectileGravityScale = 0.0f;

	UStaticMeshComponent* bulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	bulletMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	bulletMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (mesh.Succeeded())
		bulletMeshComponent->SetStaticMesh(mesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> material(TEXT("/Game/Materials/BulletMaterial.BulletMaterial"));
	if (material.Succeeded())
	{
		UMaterialInstanceDynamic* bulletMaterialInstance = UMaterialInstanceDynamic::Create(material.Object, bulletMeshComponent);
		bulletMeshComponent->SetMaterial(0, bulletMaterialInstance);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> impactSoundResource(TEXT("/Game/Audio/Sounds/Impact/Impact_Cue.Impact_Cue"));
	if (impactSoundResource.Succeeded())
		impactSound = impactSoundResource.Object;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor != ownerActor)
	{
		if (OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(bulletMovementComponent->Velocity * 100.0f, SweepResult.ImpactPoint);

		IDamagable* damagable = Cast<IDamagable>(OtherActor);
		if (damagable)
		{
			damagable->TakeDamage();
			OnHit(OtherActor);
		}

		if (impactSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), impactSound, SweepResult.ImpactPoint);

		Destroy();
	}
}

void ABullet::OnHit(AActor* OtherActor) { }

ABullet* ABullet::SpawnAndShoot(AActor* actor, TSubclassOf<class ABullet> bulletClass, FVector position, FRotator rotation, USoundCue* shootSound)
{
	UWorld* world = actor->GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = actor;
		spawnParams.Instigator = actor->GetInstigator();

		// Spawn the projectile at the muzzle.
		ABullet* bullet = world->SpawnActor<ABullet>(bulletClass, position, rotation, spawnParams);
		if (bullet)
		{
			// Set the projectile's initial trajectory.
			FVector launchDirection = rotation.Vector();
			bullet->bulletMovementComponent->Velocity = launchDirection * bullet->bulletMovementComponent->InitialSpeed;
			bullet->ownerActor = actor;

			if (shootSound)
				UGameplayStatics::PlaySoundAtLocation(world, shootSound, position, rotation);
		}

		return bullet;
	}

	return nullptr;
}