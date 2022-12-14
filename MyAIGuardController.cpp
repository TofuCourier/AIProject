// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIGuardController.h"
#include "FPSCharacter.h"
#include "MyEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"


void AMyAIGuardController::BeginPlay()
{
	Super::BeginPlay();

	PlayerChar = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	controlledPawn = Cast<AMyEnemy>(GetPawn());

	StateInitDelegate();


}

void AMyAIGuardController::StateInitDelegate()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("PAWN IN BASE STATE"));
	if (BaseDelegate.IsBound())
	{
		BaseDelegate.Broadcast();
	}

}

FBaseSignature* AMyAIGuardController::GetBaseDelegate()
{
	return &BaseDelegate;
}

void AMyAIGuardController::Tick(float DeltaSeconds)
{
	StateInitDelegate();
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, FString::SanitizeFloat(DeltaSeconds));
	}*/
	/*if (controlledPawn)
	{
    
    	switch (controlledPawn->behaviour)
    	{
    	case EEnemyBehaviour::Seeking:
    		SeekEnemy(DeltaSeconds);
    		break;
    
    	case EEnemyBehaviour::Fleeing:
    		Flee(DeltaSeconds);
    		break;
		case EEnemyBehaviour::Arriving:
			FollowAndRetreat(DeltaSeconds);
			break;
    	default:
    		break;
    	}
	
	}*/
}

bool AMyAIGuardController::Patrol(const FVector& Origin, float Radius, FVector& OutResult)
{
	FVector Result;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return false;
	}

	FVector Result;
	bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), Origin, Result, 600);

	//Out
	OutResult = Result;
	return bSuccess;
}

void AMyAIGuardController::Idle()
{
	// If standing in same position, we are idle
	FVector PawnLocation = GetPawn()->GetActorLocation();
	if (GetPawn()->GetActorLocation() == PawnLocation)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("PAWN IN IDLE STATE"));
	}

}

void AMyAIGuardController::SeekEnemy(float DeltaSeconds)
{
	FVector newLocation = FMath::VInterpTo(GetPawn()->GetActorLocation(),
		PlayerChar->GetActorLocation() ,
		DeltaSeconds,
		0.15f);

	FRotator playerRot = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), PlayerChar->GetActorLocation());

	GetPawn()->SetActorLocation(newLocation);
	GetPawn()->SetActorRotation(playerRot);

}


void AMyAIGuardController::Flee(float DeltaSeconds)
{
	FVector actorLocation = GetPawn()->GetActorLocation();
	FRotator playerRot = UKismetMathLibrary::FindLookAtRotation(actorLocation, PlayerChar->GetActorLocation());


	FVector forwardVec = UKismetMathLibrary::GetForwardVector(playerRot);
	FVector forwardVecTemp = forwardVec;
	forwardVecTemp *= -fleeSpeed;
	forwardVecTemp = forwardVecTemp + actorLocation;

	forwardVecTemp.Z = actorLocation.Z;
	GetPawn()->SetActorLocation(forwardVecTemp);
}

void AMyAIGuardController::FollowAndRetreat(float DeltaSeconds)
{

	float distToPlayer = UKismetMathLibrary::Vector_Distance2D(GetPawn()->GetActorLocation(), PlayerChar->GetActorLocation());

	FRotator defaultPlayerRot = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), PlayerChar->GetActorLocation());

	if (distToPlayer < alertRadius)
	{
		FVector actorLocation;
		FRotator playerRotFollow;
		FRotator playerRotRetreat;
		FRotator playerRotTemp;


		if (distToPlayer > stoppingDistance)
		{

			FVector newLocation = FMath::VInterpTo(GetPawn()->GetActorLocation(),
				PlayerChar->GetActorLocation(),
				DeltaSeconds,
				followSpeed);

			playerRotFollow = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), PlayerChar->GetActorLocation());

			GetPawn()->SetActorLocation(newLocation);
			GetPawn()->SetActorRotation(playerRotFollow);

			defaultPlayerRot = playerRotFollow;

		}
		

		else if (distToPlayer > stoppingDistance && distToPlayer > retreatDistance)
		{

			FVector thisLocation = GetPawn()->GetActorLocation();
			GetPawn()->SetActorLocation(thisLocation);


		}

		else if (distToPlayer < retreatDistance)
		{

			actorLocation = GetPawn()->GetActorLocation();
			playerRotRetreat = UKismetMathLibrary::FindLookAtRotation(actorLocation, PlayerChar->GetActorLocation());

			FVector forwardVec = UKismetMathLibrary::GetForwardVector(playerRotRetreat);
			FVector forwardVecTemp = forwardVec;
			forwardVecTemp *= -retreatSpeed;
			forwardVecTemp = forwardVecTemp + actorLocation;

			playerRotTemp = playerRotRetreat;
			playerRotTemp.Yaw = defaultPlayerRot.Yaw + 180.0;
			GetPawn()->SetActorRotation(playerRotTemp);

			forwardVecTemp.Z = actorLocation.Z;
			GetPawn()->SetActorLocation(forwardVecTemp);


		}
	}


	//newLocation = FMath::VInterpTo(GetPawn()->GetActorLocation(),
	//	PlayerChar->GetActorLocation(),//+ PlayerChar->GetActorForwardVector(),
	//	DeltaSeconds,
	//	0.15f);

	//playerRot = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), PlayerChar->GetActorLocation());

	//if (newLocation != FVector(0, 0, 0))
	//{
	//	/*if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("following!"));*/
	//}

	////FTransform newTransform = rota
	//GetPawn()->SetActorLocation(newLocation);
	//GetPawn()->SetActorRotation(playerRot);


	//FVector direction = PlayerChar->GetActorLocation() - GetPawn()->GetActorLocation();
	//float distance = direction.Size(); // Got the magnitude

	//if (distance)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT(" distnace is less than target"));
	//
	//
	//// Check if we are there, return no steering
	//if (distance < targetRadius)
	//{
	//	if (distance)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT(" distnace is less than target "));

	//}

	//float targetSpeed;
	//if (distance > slowRadius)
	//{
	//	targetSpeed = maxSpeed;

	//}

	//else
	//{
	//	// Use scaled speed
	//	targetSpeed = maxSpeed * distance / slowRadius;
	//}

	//// The targetVelocity combines speed and direction
	//FVector targetVelocity;
	//targetVelocity = direction;
	//targetVelocity.Normalize();

	//targetVelocity *= targetSpeed;

	//// Acceleration tries to get to the target velocity

	//FVector result = targetVelocity - PlayerChar->GetVelocity();
	//result /= timeToTarget;


	//// Clip acceleration if too high
	//if (result.Size() > maxAcceleration)
	//{
	//	result.Normalize();
	//	result *= maxAcceleration;
	//	//std::cout << "oi" << std::endl;
	//}

	//



}