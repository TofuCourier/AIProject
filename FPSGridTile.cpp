// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGridTile.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AFPSGridTile::AFPSGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootSceneComponent;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


}

// Called when the game starts or when spawned
void AFPSGridTile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSGridTile::OnBeginCursorOver()
{
	bIsHoveringCPP = true;
}

void AFPSGridTile::OnEndCursorOver()
{
	bIsHoveringCPP = false;
}

void AFPSGridTile::SetTileColorCPP()
{
	FLinearColor TileColorCPP;

	switch (GetTileDataCPP().GroundType)
	{
	case EGroundTypesCPP::Normal:
		TileColorCPP = FColor::Green;
		break;
	case EGroundTypesCPP::Hard:
		TileColorCPP = FColor::Yellow;
		break;
	case EGroundTypesCPP::Difficult:
		TileColorCPP = FColor::Orange;
		break;
	case EGroundTypesCPP::Impossible:
		TileColorCPP = FColor::Red;
		break;
	case EGroundTypesCPP::None:
		TileColorCPP = FColor::White;
		break;
	default:
		TileColorCPP = FColor::White;
		break;
	}

	if (bIsSelectedCPP)
	{
		VisualMesh->SetScalarParameterValueOnMaterials("bIsSelectedColor", 1.0f);
	}
	else
	{
		VisualMesh->SetScalarParameterValueOnMaterials("bIsSelectedColor", 0.0f);
	}

	if (bIsHoveringCPP)
	{
		TileColorCPP = TileColorCPP * 3;
	}

	if (bIsInPathCPP)
	{
		TileColorCPP = FColor::Blue;
	}

	VisualMesh->SetVectorParameterValueOnMaterials("TileColor", UKismetMathLibrary::Conv_LinearColorToVector(TileColorCPP));

}


FStructTilesCPP AFPSGridTile::GetTileDataCPP()
{
	if (GridOwner)
	{

	return GridOwner->MyStructTiles.FindRef(TileGridIndex);
	}
	
	return FStructTilesCPP();
	
}


void AFPSGridTile::DebugEvent_OpenQueueCPP()
{
	VisualMesh->SetVectorParameterValueOnMaterials("TileColor", UKismetMathLibrary::Conv_LinearColorToVector(FColor::Green));
}

