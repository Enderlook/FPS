// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 3.0f;

	if (!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	
	if (!collisionComponent)
	{
		collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		collisionComponent->InitSphereRadius(15.0f);
		collisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
		collisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
		RootComponent = collisionComponent;
	}

	if (!bulletMovementComponent)
	{
		bulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		bulletMovementComponent->SetUpdatedComponent(collisionComponent);
		bulletMovementComponent->InitialSpeed = 3000.0f;
		bulletMovementComponent->MaxSpeed = 3000.0f;
		bulletMovementComponent->bRotationFollowsVelocity = true;
		bulletMovementComponent->bShouldBounce = true;
		bulletMovementComponent->Bounciness = 0.3f;
		bulletMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!bulletMeshComponent)
	{
		bulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
		if (Mesh.Succeeded())
			bulletMeshComponent->SetStaticMesh(Mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/Materials/BulletMaterial.BulletMaterial"));
	if (Material.Succeeded())
		bulletMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, bulletMeshComponent);
	bulletMeshComponent->SetMaterial(0, bulletMaterialInstance);
	bulletMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	bulletMeshComponent->SetupAttachment(RootComponent);
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


void ABullet::FireInDirection(const FVector& shootDirection)
{
	bulletMovementComponent->Velocity = shootDirection * bulletMovementComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (OtherComponent->IsSimulatingPhysics())
			OtherComponent->AddImpulseAtLocation(bulletMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);

		for (UActorComponent* component : OtherActor->GetComponents())
		{
			if (component && component->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
			{
				Cast<IDamagable>(component)->TakeDamage();
				break;
			}
		}
	}
	Destroy();
}