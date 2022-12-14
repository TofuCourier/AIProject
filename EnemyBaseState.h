// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "MyEnemy.h"
#include "MyAIGuardControllerInterface.h"
#include "EnemyBaseState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UEnemyBaseState : public UStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	AMyEnemy* EnemyRef = nullptr;

	IMyAIGuardControllerInterface* EnemyController = nullptr;

	class AFPSCharacter* PlayerChar = nullptr;

	class AMyAIGuardController* AIEnemyController;

	virtual void OnEnterState(AActor* OwnerRef) override;
	virtual void OnExitState() override;

protected:
	virtual void BaseDelegateAction();
	
};
