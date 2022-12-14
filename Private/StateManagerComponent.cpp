// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManagerComponent.h"


// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeStates();
	
}


// Called every frame
void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanTickState)
	{
		CurrentState->TickState();
	}
	if (bDebug)
	{
		// Show current state and state history
		if (CurrentState)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, this->GetOwner()->GetName() + "'s current state " + CurrentState->StateDisplayName.GetPlainNameString());
		}
		// If we haave history
		if (StateHistory.Num() > 0)
		{
			for (int32 i = 0; i < StateHistory.Num(); i++)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, this->GetOwner()->GetName() + "'s past state" + FString::FromInt(i) + " " + StateHistory[i]->GetName());
			}
		}
	}
}

// Bind the State
void UStateManagerComponent::SwitchStateByKey(FString StateKey)
{
	// Find the state in state map
	UStateBase* NewState = StateMap.FindRef(StateKey);

	// If we found a valid state, do a normal state switch
	if (NewState->IsValidLowLevel())
	{
		// If there is no current state, we are in Initialization
		if (!CurrentState)
		{
			CurrentState = NewState;
		}
		else  // Check if the state is repeatable
		{
			// Is the new state the same as the current and is not repeatable.
			if (CurrentState->GetClass() == NewState->GetClass() && CurrentState->bCanRepeat == false)
			{
				if (bDebug)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, this->GetOwner()->GetName() + "'s state switch failed. " + CurrentState->StateDisplayName.GetPlainNameString() + " is not repeatable!", true);
				}
			}
			else // The state is repeatable
			{
				// Stop state tick
				bCanTickState = false;

				// State does this on exit -- On Exit Transition
				CurrentState->OnExitState();

				// Add current state to the state history
				if (StateHistory.Num() < StateHistoryLength)
				{
					// Push the current state to an array that will hold a pointer to every state that the actor has prev been in
					StateHistory.Push(CurrentState);
				}
				else
				{
					StateHistory.RemoveAt(0);
					StateHistory.Push(CurrentState);
				}

				// Switch current state to new state
				CurrentState = NewState;
			}
		}

		// State Initialization for New current state -- On Enter Transition
		if (CurrentState)
		{
			CurrentState->OnEnterState(GetOwner());
			bCanTickState = true;

		}
	}
	
	// No valid state found
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, this->GetOwner()->GetName() + "'s state switch failed. " + " Invalid State!");
	}
}

void UStateManagerComponent::SwitchState(UStateBase* NewState)
{
}

// Executes an initial switch state on BeginPlay
void UStateManagerComponent::InitStateManager()
{
	SwitchStateByKey(InitialState);
}

void UStateManagerComponent::InitializeStates()
{
	// Iterate through all the available states we have. Every time it finds one, creates a new object and add its to the state map.
	for (auto It = AvailableStates.CreateConstIterator(); It; ++It)
	{
		UStateBase* State = NewObject<UStateBase>(this, It->Value);
		StateMap.Add(It->Key, State);
	}
}

