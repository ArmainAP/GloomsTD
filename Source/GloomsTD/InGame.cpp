// Fill out your copyright notice in the Description page of Project Settings.

#include "InGame.h"
#include "GloomsTDGameModeBase.h"
#include "SplinePath.h"
#include "Tower.h"

void UInGame::StartWave()
{
	// We tell each Spline to start the wave we tell that there is now a wave in progress
	for (int i = 0; i < GameMode->Spline.Num(); i++)
		GameMode->Spline[i]->StartWave();
	IsWaveInProgress(true);
}

void UInGame::SetGlobalSpeed(float Speed)
{
	// Sets the fast forward speed. It is a multiplier, 1.0 is normal speed
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Speed);
}

bool UInGame::CanBuy(int Tower)
{
	// Gets the base price for the tower
	int Base = 0;
	switch (Tower)
	{
	case 1:
		Base = 20;
		break;

	case 2:
	case 3:
		Base = 25;
		break;

	case 4:
		Base = 50;
		break;
	}

	// If we have enough money, we buy the tower and return true, otherwise we return false
	if (GameMode->Money >= CalculatePrice(Base))
	{
		GameMode->BuyTower(Tower);
		IsAnotherMenuOpen = false;
		return true;
	}
	else
		return false;
}

int UInGame::CalculatePrice(int Base)
{
	// We calculate the price using the base price and we multiply it by 1.5 to the power of towers.
	return Base * FMath::Pow(1.5, GameMode->Towers);
}

bool UInGame::CanUpgrade(int Tier, int Cost)
{
	// We check if the tower can be upgraded
	if (Tier < 3 && GameMode->Money >= Cost)
		return true;
	else
		return false;
}

void UInGame::UpgradeBought(int Tier, int NewCost, UButton * Button, UTextBlock * Text)
{
	// We set the text and the color of the UI buttons based on the upgrade tier
	switch (Tier)
	{
	case 1:
		Button->SetBackgroundColor(FColor(255, 0, 0, 255));
		Text->SetText(FText::FromString("Price: " + FString::FromInt(NewCost)));
		break;

	case 2:
		Button->SetBackgroundColor(FColor(255, 255, 0, 255));
		Text->SetText(FText::FromString("Price: " + FString::FromInt(NewCost)));
		break;

	case 3:
		Button->SetBackgroundColor(FColor(255, 255, 255, 255));
		Text->SetText(FText::FromString("Maxed"));
		break;
	}
}

void UInGame::UpgradeTower()
{
	// We take away the money from the player
	GameMode->Money -= POCost;

	// We increase Player Objective's tier
	POTier++;
	// We reset the Player's Objective Damage Tier to 1
	PODamageTier = 1;
	// We reset the Player's Objective Direction Tier to 1
	PODirectionTier = 1;
	// We set the costs for the next upgrades
	POCost = 7500;
	PODirectionCost = 40 * FMath::Pow(PODirectionTier + 1, POTier);
	PODamageCost = 10 * FMath::Pow(PODamageTier + 1, POTier);

	// We set the new tier values
	GameMode->SetPOTower(POTier);
	GameMode->SetPODirection(PODirectionTier);
	GameMode->SetPODamage(PODamageTier);
}

void UInGame::UpgradeDamage()
{
	// We take away the money from the player
	GameMode->Money -= PODamageCost;
	// We increase the damage tier
	PODamageTier++;
	// We calculate the damage tier for the next upgrade
	PODamageCost = 10 * FMath::Pow(PODamageTier+1, POTier);
	// We set the damage tier
	GameMode->SetPODamage(PODamageTier);
}

void UInGame::UpgradeDirection()
{
	// We take away the money from the player
	GameMode->Money -= PODirectionCost;
	// We increase the damage tier
	PODirectionTier++;
	// We calculate the damage tier for the next upgrade
	PODirectionCost = 40 * FMath::Pow(PODirectionTier + 1, POTier);
	// We set the damage tier
	GameMode->SetPODirection(PODirectionTier);
}

bool UInGame::TowerCanUpgrade()
{
	// We check if we can upgrade the tower
	if (Tower)
	{
		if (Tower->Tier < 3 && GameMode->Money >= Tower->UpgradePrice)
		{
			// We increase the tier
			Tower->Tier++;
			// We take money away from the player
			GameMode->Money -= Tower->UpgradePrice;
			// We update the sprite
			Tower->UpdateSprite(Tower->Tier);
			// We calculate the next upgrade price
			Tower->CalculateUpgradePrice();
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void UInGame::UpdateStats(UTextBlock * Lives, UTextBlock * Wave, UTextBlock * Money)
{
	// We update information for the player about the ammount of lives, money and waves
	Lives->SetText(FText::FromString("Lives: " + FString::FromInt(GameMode->Lives)));
	Money->SetText(FText::FromString("Money: " + FString::FromInt(GameMode->Money)));
	Wave->SetText(FText::FromString("Wave: " + FString::FromInt(GameMode->Spline[0]->Wave)));
}
