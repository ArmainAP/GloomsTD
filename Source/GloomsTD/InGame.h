// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "InGame.generated.h"

class AGloomsTDGameModeBase;
class ATower;

UCLASS()
class GLOOMSTD_API UInGame : public UUserWidget
{
	GENERATED_BODY()

	// All the functions in this class are blueprint related

public:
	// Sets main menu's visibility
	UFUNCTION(BlueprintImplementableEvent)
		void MainMenuVisible();

	// Start a wave
	UFUNCTION(BlueprintCallable)
		void StartWave();

	// Checks if a wave is in progress
	UFUNCTION(BlueprintImplementableEvent)
		void IsWaveInProgress(bool IsWaveInProgress);

	// Sets the global time dilation speed
	UFUNCTION(BlueprintCallable)
		void SetGlobalSpeed(float Speed);

	// Checks if you can buy a tower
	UFUNCTION(BlueprintPure)
		bool CanBuy(int Tower);

	// Calculates the price for a tower
	UFUNCTION(BlueprintPure)
		int CalculatePrice(int Base);

	// Sets Player Objective's visibility
	UFUNCTION(BlueprintImplementableEvent)
		void POMenuVisible();

	// Check if you can upgrade a tower
	UFUNCTION(BlueprintPure)
		bool CanUpgrade(int Tier, int Cost);

	// Called when you bought an upgrade
	UFUNCTION(BlueprintCallable)
		void UpgradeBought(int Tier, int NewCost, UButton * Button, UTextBlock * Text);

	// Called when you upgrade Player Objective's tier
	UFUNCTION(BlueprintCallable)
		void UpgradeTower();

	// Called when you upgrade Player Objective's damage
	UFUNCTION(BlueprintCallable)
		void UpgradeDamage();

	// Called when you upgrade Player Objective's direction
	UFUNCTION(BlueprintCallable)
		void UpgradeDirection();

	// Sets tower menu's visibility
	UFUNCTION(BlueprintImplementableEvent)
		void TowerVisible();

	// Checks if a tower can be upgraded
	UFUNCTION(BlueprintPure)
		bool TowerCanUpgrade();

	// Updates the info displayed to the player about the ammount of lives, waves and money
	UFUNCTION(BlueprintCallable)
		void UpdateStats(UTextBlock* Lives, UTextBlock* Wave, UTextBlock* Money);

	// Called when the player lost the game
	UFUNCTION(BlueprintImplementableEvent)
		void ShowGameOver();

public:
	UPROPERTY()
		AGloomsTDGameModeBase* GameMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATower* Tower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAnotherMenuOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int POTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int POCost = 2500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PODamageTier = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PODamageCost = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PODirectionTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PODirectionCost = 80;
};
