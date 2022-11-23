// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

#include "FPSGrid.generated.h"

class AFPSGridTile;

UENUM(BlueprintType)
enum class EGroundTypesCPP : uint8 {
	Normal	UMETA(DisplayName = "Normal"),
	Hard UMETA(DisplayName = "Hard"),
	Difficult UMETA(DisplayName = "Difficult"),
	Impossible UMETA(DisplayName = "Impossible"),
	None UMETA(DisplayName = "None")
};



USTRUCT(BlueprintType)
struct FStructTilesCPP
{
	GENERATED_BODY()

//		FStructTilesCPP();
	//FStructTilesCPP(AFPSGridTile* tile, EGroundTypesCPP ground, FVector worldPos, FVector2D index, int tileCost, int finalCost, int costFromStart, int estCostToTarget, FVector2D prevTile);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFPSGridTile* TileActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGroundTypesCPP GroundType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D GridIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FinalCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CostFromStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EstCostToTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PreviousTile;

};



UCLASS()
class FPSGAME_API AFPSGrid : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AFPSGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	FLinearColor GridBoxColour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float TileSize = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineComp;

	FVector TileNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	AFPSGridTile* selectedTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	AFPSGridTile* hoveredTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FVector2D, FStructTilesCPP> MyStructTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> PathCPP;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector GridLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Cost;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector BotLeftCornerGrid();

	UFUNCTION(BlueprintCallable)
	FVector GridTileNum();

	UFUNCTION(BlueprintCallable)
	void DrawTile();

	UFUNCTION(BlueprintCallable)
	int GetTileCostCPP(EGroundTypesCPP groundType); //******** our function for the a* algorithm -- groundType aka gridIndex

	// Our A* algorithm !!!!!!!
	UFUNCTION(BlueprintCallable)
	void AStarPathfinding(FVector2D start, FVector2D target);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FVector2D> GetTileNeighboursCPP(FVector2D gridIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetEstimCostToTargetCPP(FVector2D current, FVector2D target);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FVector2D> RetracePathCPP(FVector2D targetTile, FVector2D startTile);

	UFUNCTION(BlueprintCallable)
	void HighlightCurrentPathCPP(bool bIsForHighlight);



};
