// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGrid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "FPSGridTile.h"

// Sets default values
AFPSGrid::AFPSGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridSize.X = 1000;
	GridSize.Y = 1000;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootSceneComponent;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliComp"));
	CollisionComp->SetupAttachment(RootSceneComponent);
	//CollisionComp->InitSphereRadius(5.0f);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->SetupAttachment(RootSceneComponent);
}

// Called when the game starts or when spawned
void AFPSGrid::BeginPlay()
{
	Super::BeginPlay();
	
		FlushPersistentDebugLines(GetWorld());

		DrawDebugBox(GetWorld(), GridLocation, GridSize, FColor::Blue, false, 1000000.0f, 10.0f);
		DrawDebugBox(GetWorld(), BotLeftCornerGrid(), FVector(15.0f, 15.0f, 15.0f), FColor::Green, false, 1000000.0f, 20.0f);
		TileNum = GridTileNum();
		GridLocation = RootComponent->GetRelativeLocation();
}

// Called every frame
void AFPSGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AFPSGrid::BotLeftCornerGrid()
{
	FVector tempX;
	tempX = RootComponent->GetRightVector() * GridSize.X; //Lets get the vector to the right of us * the
	tempX = GridLocation - tempX;

	FVector tempY;
	tempY = RootComponent->GetForwardVector() * GridSize.Y;
	tempX = tempX - tempY;

	return FVector(tempX);
}

FVector AFPSGrid::GridTileNum()
{
	FVector temp;
	temp = GridSize / TileSize;
	temp.X = temp.X - 1;
	round(temp.X);
	temp.Y = temp.Y - 1;
	round(temp.Y);
	return FVector(temp);
}

void AFPSGrid::DrawTile()
{
	TileNum = GridTileNum();

	int truncX = UKismetMathLibrary::FTrunc(TileNum.X);
	int truncY = UKismetMathLibrary::FTrunc(TileNum.Y);

	int varX;
	int varY;


	for (int i = 0; i < truncX; i++)
	{
		varX = i;

		for (int j = 0; j < truncY; j++)
		{
			varY = j;
			
			float TileSizeTemp1 = TileSize * 2.0f;
			TileSizeTemp1 = TileSizeTemp1 * varX;
			TileSizeTemp1 = TileSizeTemp1 + TileSize;
			FVector tempPos1;
			tempPos1 = RootComponent->GetRightVector();
			tempPos1 = tempPos1 * TileSizeTemp1;

			float TileSizeTemp2 = TileSize * 2.0f;
			TileSizeTemp2 = TileSizeTemp2 * varY;
			TileSizeTemp2 = TileSizeTemp2 + TileSize;
			FVector tempPos2 = RootComponent->GetForwardVector();
			tempPos2 = tempPos2 * TileSizeTemp2;

			FVector TilePos;
			FVector BLC = BotLeftCornerGrid();
			TilePos = BLC + tempPos1 + tempPos2;

			DrawDebugSolidPlane(GetWorld(), FPlane(0.0f, 0.0f, 1.0f, TilePos.Z), TilePos, TileSize - 5, FColor::Purple, false, 10000000.0);
			//DrawDebugSolidPlane(GetWorld(), FPlane(0.0f, 0.0f, 1.0f, TilePos.Z), FVector(0.0f,0.0f,0.0f), TileSize, FColor::Green, false, 10000000.0);

		}
	}
}

int AFPSGrid::GetTileCostCPP(EGroundTypesCPP groundType)
{
	switch (groundType)
	{
		case EGroundTypesCPP::Normal:
			Cost = 1;
			break;

		case EGroundTypesCPP::Hard:
			Cost = 2;
			break;
		case EGroundTypesCPP::Difficult:
			Cost = 5;			
			break;
		case EGroundTypesCPP::Impossible:
			Cost = 999;
			break;
		case EGroundTypesCPP::None:
			Cost = 999;
			break;
		default:
			break;
	}
	return Cost;
}


void AFPSGrid::AStarPathfinding(FVector2D start, FVector2D target)
{
	FVector2D StartTemp;
	FVector2D TargetTemp;
	FVector2D Current;
	FVector2D OpenQLowestCost;
	FVector2D CurrentNeighbour;

	TArray<FVector2D> OpenQueue;
	TArray<FVector2D> ClosedQueue;

	int CurNeighbourCostFromStart = 0;



	StartTemp = start;
	TargetTemp = target;

	// Making sure we have valid, passable struct tiles
	if (MyStructTiles.Find(TargetTemp) &&
		MyStructTiles[TargetTemp].GroundType != EGroundTypesCPP::None &&
		MyStructTiles[TargetTemp].GroundType != EGroundTypesCPP::Impossible)
	{
		//// First thing is to populate the tiles with their default values

		//// Create a key array holding our struct data
		TArray<FVector2D> TileArray;
		MyStructTiles.GenerateKeyArray(TileArray);


		for (auto i : TileArray)
		{
			MyStructTiles.Find(i);

			MyStructTiles[i].FinalCost = 0;
			MyStructTiles[i].CostFromStart = 0;
			MyStructTiles[i].EstCostToTarget = 0;
			MyStructTiles[i].PreviousTile = FVector2D(0, 0);

			MyStructTiles.Add(i);
		}

		//changing these values for default that arent used currently - can delete this later probably?
		MyStructTiles.Find(StartTemp);
		int tempCostToTarget = GetEstimCostToTargetCPP(StartTemp, TargetTemp);
		MyStructTiles[StartTemp].FinalCost = tempCostToTarget;
		MyStructTiles[StartTemp].CostFromStart = 0;
		MyStructTiles[StartTemp].EstCostToTarget = tempCostToTarget;
		MyStructTiles[StartTemp].PreviousTile = FVector2D(0, 0);  /////////////////

		MyStructTiles.Add(StartTemp);

		Current = StartTemp;
		OpenQueue.AddUnique(StartTemp);
		
		while (OpenQueue.Num() > 0)
		{
			if (OpenQueue.Num() == 0)
			{
				return;
			}

			OpenQLowestCost = OpenQueue[0]; // could be wrong , get data

			for (auto i : OpenQueue)
			{
				int QueueFinalCost = MyStructTiles[i].FinalCost;
				int QueueEstCost = MyStructTiles[i].EstCostToTarget;
				int OpenLowestFinalCost = MyStructTiles[OpenQLowestCost].FinalCost;
				int OpenLowestEstCost = MyStructTiles[OpenQLowestCost].EstCostToTarget;

				if (QueueFinalCost < OpenLowestFinalCost)
				{
					OpenQLowestCost = i;
					// should i return after since we have lowest cost to get out of loop?
				}
				else if (QueueFinalCost == OpenLowestFinalCost)
				{
					if (QueueEstCost < OpenLowestEstCost)
					{
						OpenQLowestCost = i;

					}
				}

			}
			Current = OpenQLowestCost;
			OpenQueue.Remove(Current);
			ClosedQueue.AddUnique(Current);
			for (auto i : GetTileNeighboursCPP(Current))
			{
				CurrentNeighbour = i;

				// If the closed queue does not have the current neighbour
				if (!ClosedQueue.Contains(CurrentNeighbour))
				{
					// Get the tile cost of new tile and add it to our current cost so far
					CurNeighbourCostFromStart = MyStructTiles[Current].CostFromStart + MyStructTiles[CurrentNeighbour].TileCost;
					if (!OpenQueue.Contains(CurrentNeighbour))
					{
						OpenQueue.AddUnique(CurrentNeighbour);

						MyStructTiles[CurrentNeighbour].FinalCost = CurNeighbourCostFromStart
							+ GetEstimCostToTargetCPP(CurrentNeighbour, TargetTemp);
						MyStructTiles[CurrentNeighbour].CostFromStart = CurNeighbourCostFromStart;
						MyStructTiles[CurrentNeighbour].EstCostToTarget = GetEstimCostToTargetCPP(CurrentNeighbour, TargetTemp);
						MyStructTiles[CurrentNeighbour].PreviousTile = Current;
						MyStructTiles.Add(CurrentNeighbour);


					}
					else if (CurNeighbourCostFromStart < MyStructTiles[CurrentNeighbour].CostFromStart)
					{
						MyStructTiles[CurrentNeighbour].FinalCost = CurNeighbourCostFromStart
							+ GetEstimCostToTargetCPP(CurrentNeighbour, TargetTemp);
						MyStructTiles[CurrentNeighbour].CostFromStart = CurNeighbourCostFromStart;
						MyStructTiles[CurrentNeighbour].EstCostToTarget = GetEstimCostToTargetCPP(CurrentNeighbour, TargetTemp);
						MyStructTiles[CurrentNeighbour].PreviousTile = Current;
						MyStructTiles.Add(CurrentNeighbour);
					}
					else
					{
						return;
					}

					//MyStructTiles.Add(CurrentNeighbour);

					if (UKismetMathLibrary::EqualEqual_Vector2DVector2D(CurrentNeighbour, TargetTemp, 0.0001)) //UKismetMathLibrary::EqualEqual_Vector2DVector2D(CurrentNeighbour, TargetTemp, 0.0001)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT(" =="));

						PathCPP = RetracePathCPP(TargetTemp, StartTemp);
					}
				}
			}
			
		}
	}
}

TArray<FVector2D> AFPSGrid::GetTileNeighboursCPP(FVector2D gridIndex)
{
	// Our Local Variables
	TArray<FVector2D> PossibleNeighbours;
	TArray<FVector2D> outNeighbours; // For the output

	PossibleNeighbours.Add(FVector2D(1, 0));
	PossibleNeighbours.Add(FVector2D(0, 1));
	PossibleNeighbours.Add(FVector2D(-1, 0));
	PossibleNeighbours.Add(FVector2D(0, -1));

	// Lets loop through our 4 neighbours on the x/y axis  (z is up)
	for (auto i : PossibleNeighbours)
	{
		FVector2D tempIndex;
		tempIndex = i + gridIndex;

		// Find the ground Type of the tiles in the struct so we get the ones we can traverse.
		//StructTiles.FindRef(tempIndex);
		if (MyStructTiles.Find(tempIndex)) 
		{
			if (MyStructTiles[gridIndex].GroundType != EGroundTypesCPP::None &&
				MyStructTiles[gridIndex].GroundType != EGroundTypesCPP::Impossible)
			{
				outNeighbours.AddUnique(tempIndex);
			}
		}
	}

	return outNeighbours;
}

int AFPSGrid::GetEstimCostToTargetCPP(FVector2D current, FVector2D target)
{
	FVector2D temp;
	temp = current - target;

	float tempX = temp.X;
	int tempY = temp.Y;
	tempX = abs(tempX);
	tempY = abs(tempY);

	int tempRounded;
	tempRounded = tempX + tempY;
	tempRounded = roundf(tempRounded); //  hope this is right
	return tempRounded;
}

TArray<FVector2D> AFPSGrid::RetracePathCPP(FVector2D targetTile, FVector2D startTile)
{
	FVector2D Current;
	TArray<FVector2D> OutPath;
	TArray<FVector2D> ReversePath;

	Current = targetTile;
	while (UKismetMathLibrary::NotEqual_Vector2DVector2D(Current, startTile, 0.0001))
	{
		ReversePath.AddUnique(Current);
		Current = MyStructTiles[Current].PreviousTile;
	}
	// Cycle through each tile in the path, in reverse
	for (auto i = ReversePath.Num(); i--;) // does this reverse for each loop work?
	{
		OutPath.AddUnique(ReversePath[i]);
	}
	
	return OutPath;
}

void AFPSGrid::HighlightCurrentPathCPP(bool bIsForHighlight)
{
	for (auto i : PathCPP)
	{
		AFPSGridTile* tileActor = MyStructTiles[i].TileActor;
		if (tileActor)
		{
			tileActor->Event_IsInPathCPP(bIsForHighlight);
		}
		
	}
}



