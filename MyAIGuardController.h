// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIGuardController.generated.h"

/**
 * 
 */
UCLASS()


class FPSGAME_API AMyAIGuardController : public AAIController
{
	GENERATED_BODY()

protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

public:

		//AFPSCharacter* PlayerChar;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Behaviour")
		float agroRadius = 50;

		FVector newLocation;
		FRotator playerRot;

		UFUNCTION (BlueprintCallable)
		void SeekEnemy(float DeltaSeconds);

		UFUNCTION()
		void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
		UBehaviorTree* EnemyBT;

		virtual void Tick(float DeltaSeconds);




	
};
