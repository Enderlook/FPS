// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Bullet.h"
#include "Damagable.h"
#include "TPS_Player.generated.h"

UCLASS()
class FPS_API ATPS_Player : public ACharacter, public IDamagable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPS_Player();

private:
	UPROPERTY(EditAnywhere)
	int hitpoints;
	int currentHitpoints;

	UPROPERTY(EditAnywhere)
	int hitpointsRestoredOnPickup;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
	TSubclassOf<UUserWidget> playerPowerWidgetClass;
	UUserWidget* playerPowerWidget;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	UCameraComponent* followCamera;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USkeletalMeshComponent* Firingdrone;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void TakeDamage() override;

	virtual void Jump() override;

private:
	UFUNCTION()
	void OnBeginOverlap(
		class UPrimitiveComponent* HitComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void Fire();

	void MoveForward(float Axis);

	void MoveRight(float Axis);

	void RestartGame();
};
