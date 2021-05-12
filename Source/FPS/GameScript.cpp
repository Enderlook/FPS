// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScript.h"
#include "Kismet/GameplayStatics.h"

void AGameScript::OnEnemySpawned()
{
	enemyCount++;
}

void AGameScript::OnEnemyDestroyed()
{
	if (--enemyCount == 0)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AGameScript::ContinueGame, 3.0f, false);
	}
}

void AGameScript::ContinueGame()
{
	if (levelName == FName("None"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Congratulations! You completed the game!"));
	}
	else
		UGameplayStatics::OpenLevel(this, levelName);
}