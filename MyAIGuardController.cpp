// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIGuardController.h"
#include "FPSCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Rotator.h"
#include "MyAIGuardController.h"


void AMyAIGuardController::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyBT)
	{
	RunBehaviorTree(EnemyBT);
	}



}

void AMyAIGuardController::Tick(float DeltaSeconds)
{

	//move this to OnBeginPlay: RunBehaviorTree(EnemyBT);

	//GetPawn()->OnActorBeginOverlap.AddDynamic(this, &AMyEnemy::HandleOverlap);

	
	
	SeekEnemy(DeltaSeconds);
}

void AMyAIGuardController::SeekEnemy(float DeltaSeconds)
{
	
	AFPSCharacter* PlayerChar = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (PlayerChar)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("following!"));
	FVector distance = PlayerChar->GetActorLocation() - GetPawn()->GetActorLocation();

	newLocation = FMath::VInterpTo(GetPawn()->GetActorLocation(),
		PlayerChar->GetActorLocation() + PlayerChar->GetActorForwardVector(),
		DeltaSeconds,
		0.15f);

	playerRot = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), PlayerChar->GetActorLocation());

	if (newLocation != FVector(0, 0, 0))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("following!"));
	}

	//FTransform newTransform = rota
	GetPawn()->SetActorLocation(newLocation);
	GetPawn()->SetActorRotation(playerRot);

	

}

void AMyAIGuardController::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
