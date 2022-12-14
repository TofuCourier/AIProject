// Fill out your copyright notice in the Description page of Project Settings.


#include "SeekEnemyState.h"
#include "FPSCharacter.h"
#include "MyAIGuardController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void USeekEnemyState::BaseDelegateAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("Seeking"));


	AIEnemyController->SeekEnemy(0.5);
}

void USeekEnemyState::TickState()
{
	Super::TickState();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("Seeking"));

	
	
	float distToPlayer = UKismetMathLibrary::Vector_Distance2D(EnemyRef->GetActorLocation(), PlayerChar->GetActorLocation());
	if (distToPlayer > agroRadius)
	{

		EnemyRef->StateManager->SwitchStateByKey("Idle");
	}


}
