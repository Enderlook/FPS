// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Engine/LevelScriptActor.h"
#include "Sound/SoundCue.h"
#include "GameScript.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AGameScript : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName levelName;

	UPROPERTY(EditAnywhere)
	USoundCue* backgroundMusic;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> loseWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> winWidgetClass;

	UAudioComponent* backgroundMusicComponent;

	int enemyCount;

protected:
	virtual void BeginPlay() override;

private:
	UUserWidget* MakeWidget(TSubclassOf<UUserWidget> widgetClass);

public:
	void OnEnemySpawned();
	void OnEnemyDestroyed();
	void OnLostGame();
	void NextLevel();
};
