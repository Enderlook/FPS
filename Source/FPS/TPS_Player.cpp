#include "TPS_Player.h"
#include  "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include  "Kismet/GameplayStatics.h"

// Sets default values
ATPS_Player::ATPS_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0, 96.0);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	UCharacterMovementComponent* movement = GetCharacterMovement();
	movement->bOrientRotationToMovement = true;
	movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	movement->JumpZVelocity = 650.0f;
	movement->AirControl = 0.2f;

	USpringArmComponent* cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);

	cameraBoom->TargetArmLength = 300.0f;
	cameraBoom->bUsePawnControlRotation = true;

	UCameraComponent* followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName); 
	followCamera->bUsePawnControlRotation = false;

	bDead = false;
	power = 100.0f;	
}

// Called when the game starts or when spawned
void ATPS_Player::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPS_Player::OnBeginOverlap);
	if (playerPowerWidgetClass != nullptr)
	{
		playerPowerWidget = CreateWidget(GetWorld(), playerPowerWidgetClass);
		playerPowerWidget->AddToViewport();
	}
}

// Called every frame
void ATPS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	power -= DeltaTime * powerThreshold;

	if (power <= 0)
	{
		if (!bDead)
		{
			bDead = true;
			
			GetMesh()->SetSimulatePhysics(true);
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATPS_Player::RestartGame, 3.0f, false);
		}
	}
}

// Called to bind functionality to input
void ATPS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPS_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPS_Player::MoveRight);
}

void ATPS_Player::MoveForward(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void ATPS_Player::MoveRight(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void ATPS_Player::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName(), false));
}

void ATPS_Player::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Recharge"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Collided with Recharge"));

		power += 10.0f;

		if (power > 100.0f)
			power = 100.0f;

		OtherActor->Destroy();		
	}
}