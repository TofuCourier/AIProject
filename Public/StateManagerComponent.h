// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateBase.h"
#include "StateManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUSTOMSTATEMACHINE_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// String to define the initial state the actor will be in
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State Machine")
	FString InitialState;
	
	// The available state of statemanagercomponent
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State Machine")
	TMap<FString, TSubclassOf<UStateBase>> AvailableStates;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State Machine Debug")
	bool bDebug = false;
	
	// Used to see the history of the states.  Also StateHistory's Length
	UPROPERTY(BlueprintReadOnly)
	TArray<UStateBase*> StateHistory;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State Machine Debug", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	int32 StateHistoryLength;
	
	UPROPERTY(BlueprintReadOnly)
	UStateBase* CurrentState = nullptr;
	
	UPROPERTY()
	TMap<FString, UStateBase*> StateMap;  // Uses UstateBase pointer, different from AvailableStates
	
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void SwitchStateByKey(FString StateKey);
	
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void SwitchState(UStateBase* NewState);
	
	
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void InitStateManager();




private:
	bool bCanTickState = false;
	void InitializeStates();
};
