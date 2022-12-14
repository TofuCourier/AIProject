// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseState.h"
#include "IdleEnemyState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UIdleEnemyState : public UEnemyBaseState
{
	GENERATED_BODY()



protected:
	virtual void BaseDelegateAction() override;
	virtual void TickState() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
		float agroRadius = 1000.0f;
};
