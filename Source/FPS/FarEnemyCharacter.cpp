// Fill out your copyright notice in the Description page of Project Settings.


#include "FarEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"

// Called every frame
bool AFarEnemyCharacter::CanAttack()
{
	if (!Super::CanAttack())
		return false;

	AEnemyAIController* controller = GetAIController();
	AActor* player_ = GetPlayer();
	if (player_ && controller && IsPlayerInSight())
	{
		float distance = player_->GetDistanceTo(this);
		if (distance < minPlayerDistance)
		{
			UWorld* world = GetWorld();
			if (world)
			{
				UNavigationSystemV1* navigationSystem = UNavigationSystemV1::GetNavigationSystem(world);
				if (navigationSystem)
				{
					FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player_->GetActorLocation());
					FVector forwardVector = UKismetMathLibrary::GetForwardVector(lookRotation);
					forwardVector.Z = 0;

					float fleeDistance = -(minPlayerDistance - distance + 25);
					FVector fleeDirection = forwardVector * fleeDistance;
					FVector myLocation = GetActorLocation();
					FVector newLocation = myLocation + fleeDirection;

					FNavLocation navigationLocation;
					
					if (navigationSystem->GetRandomReachablePointInRadius(newLocation, 15, navigationLocation))
						controller->MoveToLocation(navigationLocation.Location, 5, false);
					else
					{
						// Fallback because random reachable point is buggy.

						if (controller->MoveToLocation(newLocation, 5, false) == EPathFollowingRequestResult::Type::Failed)
							if (!controller->MoveToLocation((myLocation + (forwardVector * fleeDistance / 2)), 5, false) == EPathFollowingRequestResult::Type::Failed)
								controller->MoveToLocation((myLocation + (forwardVector * fleeDistance / 4)), 5, false);

						// TODO: If none of those locations is valid, should fallback to diagonals?
					}

					return false;
				}
			}
		}
	}

	return true;
}