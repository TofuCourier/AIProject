
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSGrid.h"
#include "FPSGridTile.generated.h"


UCLASS()
class FPSGAME_API AFPSGridTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
		USceneComponent* RootSceneComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* VisualMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		FVector2D TileGridIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		class AFPSGrid* GridOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		bool bIsHoveringCPP = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		bool bIsSelectedCPP = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		bool bIsInPathCPP = false;

	void OnBeginCursorOver();

	void OnEndCursorOver();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Event_SelectTile();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Event_DeselectTile();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Event_EndOverTile();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Event_IsInPathCPP(bool bIsInPath);

	UFUNCTION(BlueprintCallable)
		void SetTileColorCPP();

	UFUNCTION(BlueprintCallable)
		FStructTilesCPP GetTileDataCPP();

	//// DUBUGGGGGG///
	UFUNCTION(BlueprintCallable)
		void DebugEvent_OpenQueueCPP();

};
