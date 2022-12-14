// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CUSTOMSTATEMACHINE_API UStateBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool bCanTickState = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool bCanRepeat = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FName StateDisplayName;

public:
	virtual void OnEnterState(AActor* StateOwner);
	virtual void OnExitState();
	virtual void TickState();  // The condition to get out of the state
	
};
