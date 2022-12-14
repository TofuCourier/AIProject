// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleEnemyState.h"
#include "FPSCharacter.h"
#include "MyAIGuardController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



void UIdleEnemyState::BaseDelegateAction()  // The action
{
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("IDLE"));
	AIEnemyController->Idle();
	

}

void UIdleEnemyState::TickState() // The condition to switch out
{
	Super::TickState();
	float distToPlayer = UKismetMathLibrary::Vector_Distance2D(EnemyRef->GetActorLocation(), PlayerChar->GetActorLocation());
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("IDLE"));
	if (distToPlayer < agroRadius)
	{
		EnemyRef->StateManager->SwitchStateByKey("Seek");
	}

}

