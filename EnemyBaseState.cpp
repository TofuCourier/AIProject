// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseState.h"
#include "MyAIGuardControllerInterface.h"
#include "GameFramework/Pawn.h"
#include "MyAIGuardController.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

void UEnemyBaseState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

	// If no Player Ref, get one. Save player Ref for later
	if (!EnemyRef)
	{
		{
			EnemyRef = Cast<AMyEnemy>(OwnerRef);
		}
	}

	if (!AIEnemyController)
	{
		AIEnemyController = Cast<AMyAIGuardController>(EnemyRef->GetController()); // Now we have access to controller and the delegate
	}

	if (!PlayerChar)
	{
		PlayerChar = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	}

	if (!EnemyController)
	{
		EnemyController = Cast<IMyAIGuardControllerInterface>(EnemyRef->GetController()); // Now we have access to controller and the delegate
	}

	if (EnemyController)
	{
		EnemyController->GetBaseDelegate()->AddUObject(this, &UEnemyBaseState::BaseDelegateAction);  // When we enter the game this function will trigger
	}
}

void UEnemyBaseState::OnExitState()
{
	Super::OnExitState();
	// Every time we switch state we will unbind everything from this state (as seen above) and the next scene can bind the delegate
	EnemyController->GetBaseDelegate()->RemoveAll(this);
}

void UEnemyBaseState::BaseDelegateAction()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("base state class basedelegateaction on enter game"));
}
