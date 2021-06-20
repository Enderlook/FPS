#include "TPS_Player.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATPS_Player::ATPS_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0, 96.0);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	movementComponent = GetCharacterMovement();
	if (movementComponent)
	{
		movementComponent->bOrientRotationToMovement = true;
		movementComponent->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
		movementComponent->JumpZVelocity = 650.0f;
		movementComponent->AirControl = 0.2f;
		initialMaxWalkSpeed = movementComponent->MaxWalkSpeed;
		initialMaxWalkSpeedCrouched = movementComponent->MaxWalkSpeedCrouched;
		initialMaxAcceleration = movementComponent->MaxAcceleration;
		speedMultiplier = 1;
	}

	USpringArmComponent* cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);

	cameraBoom->TargetArmLength = 300.0f;
	cameraBoom->SocketOffset = FVector(0.0f, 150.0f, 0.0f);
	//cameraBoom->AddRelativeLocation(FVector(0.0f, 300.0f, 0.0f));
	cameraBoom->bUsePawnControlRotation = true;

	UCameraComponent* followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = false;

	firingDrone = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Drone2"));
	firingDrone->SetRelativeLocation(FVector(cameraBoom->TargetArmLength, 0.0f, 100.0f));
	firingDrone->SetupAttachment(cameraBoom);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/Meshes/drone.drone"));
	if (mesh.Succeeded())
	{
		firingDrone->SetSkeletalMesh(mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> material(TEXT("/Game/ParagonGadget/FX/Materials/Hero_Specific/M_Gadget_Gadgets.M_Gadget_Gadgets"));
		if (material.Succeeded())
		{
			UMaterialInstanceDynamic* droneMaterialInstance = UMaterialInstanceDynamic::Create(material.Object, firingDrone);
			firingDrone->SetMaterial(0, droneMaterialInstance);
		}
	}
}

// Called when the game starts or when spawned
void ATPS_Player::BeginPlay()
{
	Super::BeginPlay();

	if (playerPowerWidgetClass != nullptr)
	{
		playerPowerWidget = CreateWidget(GetWorld(), playerPowerWidgetClass);
		playerPowerWidget->AddToViewport();
	}

	currentHitpoints = hitpoints;
}

// Called every frame
void ATPS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (firingDrone)
		firingDrone->SetWorldRotation(GetControlRotation());
}

// Called to bind functionality to input
void ATPS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &ATPS_Player::RestartGame);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPS_Player::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPS_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPS_Player::MoveRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPS_Player::Fire);
}

void ATPS_Player::Fire()
{
	if (currentHitpoints <= 0)
		return;

	if (bulletClass)
	{
		// Get the camera transform.
		FVector cameraLocation;
		FRotator cameraRotation;
		GetActorEyesViewPoint(cameraLocation, cameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		FVector muzzleOffset = FVector(0.0f, 150.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector muzzleLocation = cameraLocation + FTransform(cameraRotation).TransformVector(muzzleOffset);
		
		// Skew the aim to be slightly upwards.
		FRotator muzzleRotation = cameraRotation;

		ABullet::SpawnAndShoot(this, bulletClass, muzzleLocation, muzzleRotation);
	}
}

void ATPS_Player::MoveForward(float Axis)
{
	if (currentHitpoints > 0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void ATPS_Player::MoveRight(float Axis)
{
	if (currentHitpoints > 0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void ATPS_Player::Jump()
{
	if (currentHitpoints > 0)
		Super::Jump();
}


void ATPS_Player::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName(), false));
}

void ATPS_Player::TakeDamage()
{
	if (--currentHitpoints <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		firingDrone->SetSimulatePhysics(true);
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &ATPS_Player::RestartGame, 3.0f, false);
	}
}

void ATPS_Player::RestoreHitpoints(int restoredHitpoints)
{
	currentHitpoints += restoredHitpoints;
		
	if (currentHitpoints > hitpoints)
		currentHitpoints = hitpoints;
}

void ATPS_Player::ModifySpeed(float factor, float duration)
{
	speedMultiplier *= factor;

	if (movementComponent)
	{
		movementComponent->MaxWalkSpeed = initialMaxWalkSpeed * speedMultiplier;
		movementComponent->MaxWalkSpeedCrouched = initialMaxWalkSpeedCrouched * speedMultiplier;
		movementComponent->MaxAcceleration = initialMaxAcceleration * speedMultiplier;

		FTimerHandle handle;
		FTimerDelegate slowdownDelegate = FTimerDelegate::CreateUObject(this, &ATPS_Player::UnmodifySpeed, factor);
		GetWorldTimerManager().SetTimer(handle, slowdownDelegate, duration, false);
	}
}

void ATPS_Player::UnmodifySpeed(float factor)
{
	speedMultiplier /= factor;

	if (movementComponent)
	{
		movementComponent->MaxWalkSpeed = initialMaxWalkSpeed * speedMultiplier;
		movementComponent->MaxWalkSpeedCrouched = initialMaxWalkSpeedCrouched * speedMultiplier;
		movementComponent->MaxAcceleration = initialMaxAcceleration * speedMultiplier;
	}
}