// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PaperSprite.h"
#include "InGame.h"
#include "GloomsTDGameModeBase.generated.h"

class ASplinePath;
class APlayerObjective;
class ATower;

UCLASS()
class GLOOMSTD_API AGloomsTDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGloomsTDGameModeBase();
	// Shows the main menu
	void ShowMainMenu();
	// Buys a tower
	void BuyTower(int Tower);
	// Spawns the tower you bought
	void BuildingMode(int Tower);

	// Shows the upgrade menu for the Player Objective
	void ShowPOMenu();
	// Sets the Player Objective damage tier
	void SetPODamage(int Tier);
	// Sets the Player Objective direction tier
	void SetPODirection(int Tier);
	// Sets the Player Objective tier
	void SetPOTower(int Tier);

	// Shows the upgrade menu for a clicked tower
	void ShowTowerMenu(ATower* Tower);

	// Called when one of the splines ended its wave
	void EndWave();
	// Called when the player loses a life
	void LoseLife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	int EndedWaves = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Lives = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Money = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Towers = 0;

	UPROPERTY()
		APlayerObjective* PlayerObjective;

	UPROPERTY()
		UInGame* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
		TSubclassOf<UInGame> WidgetClass;

	UPROPERTY()
		TArray<ASplinePath*> Spline;

	UPROPERTY()
		TArray<UPaperSprite*> POSprite;
};