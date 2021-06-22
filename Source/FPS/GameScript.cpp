// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScript.h"
#include "HasBackgroundSound.h"
#include "HasNextLevelButton.h"
#include "Kismet/GameplayStatics.h"

void AGameScript::BeginPlay()
{
	if (backgroundMusic)
		backgroundMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), backgroundMusic);
}

void AGameScript::OnEnemySpawned()
{
	enemyCount++;
}

void AGameScript::OnEnemyDestroyed()
{
	if (--enemyCount == 0)
	{
		UUserWidget* widget = MakeWidget(winWidgetClass);
		if (widget && widget->Implements<UHasNextLevelButton>())
			IHasNextLevelButton::Execute_SetNextLevelName(widget, levelName);
	}
}

void AGameScript::NextLevel()
{
	UGameplayStatics::OpenLevel(this, levelName);
}

void AGameScript::OnLostGame()
{
	MakeWidget(loseWidgetClass);
}

UUserWidget* AGameScript::MakeWidget(TSubclassOf<UUserWidget> widgetClass)
{
	if (!widgetClass)
		return nullptr;

	APlayerController* player = GetWorld()->GetFirstPlayerController();
	player->SetPause(true);
	player->SetShowMouseCursor(true);
	
	UUserWidget* widget = CreateWidget(GetWorld(), widgetClass);
	widget->AddToViewport();

	if (widget->Implements<UHasBackgroundSound>())
	{
		USoundCue* sound = IHasBackgroundSound::Execute_GetBackgroundSound(widget);
		if (sound)
		{
			if (backgroundMusicComponent)
			{
				backgroundMusicComponent->Stop();
				backgroundMusicComponent->SetSound(sound);
				backgroundMusicComponent->Play();
			}
			else
				backgroundMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), sound);
		}
	}

	return widget;
}