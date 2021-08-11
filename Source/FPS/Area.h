// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Area.generated.h"

UCLASS()
class FPS_API AArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArea();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	float warmupTime;

	UPROPERTY(EditAnywhere, Category = "Area")
	float duration;

	UPROPERTY(EditAnywhere, Category = "Area")
	float fireRateSlowdownFactor;

	UPROPERTY(BlueprintReadOnly, Category="Area")
	UStaticMeshComponent* areaMeshComponent;

private:
	//UBoxComponent* collisionComponent;
	USoundCue* affectSound;
	USoundCue* unaffectSound;
	bool hasPlayer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Initialize();
	UFUNCTION()
	void Deinitialize();
	void ManualSetPlayer(bool apply);
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
