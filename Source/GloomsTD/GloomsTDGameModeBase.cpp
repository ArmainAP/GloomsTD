// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomsTDGameModeBase.h"
#include "SplinePath.h"
#include "PlayerObjective.h"
#include "SPE.h"
#include "Mirror.h"
#include "Prism.h"
#include "EMP.h"

AGloomsTDGameModeBase::AGloomsTDGameModeBase()
{
	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundPOSprite0(TEXT("/Game/Assets/Player/PlayerObjectiveSprite"));
	if (FoundPOSprite0.Succeeded())
		POSprite.Add(FoundPOSprite0.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundPOSprite1(TEXT("/Game/Assets/Player/PlayerObjectiveSprite2"));
	if (FoundPOSprite1.Succeeded())
		POSprite.Add(FoundPOSprite1.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundPOSprite2(TEXT("/Game/Assets/Player/PlayerObjectiveSprite3"));
	if (FoundPOSprite2.Succeeded())
		POSprite.Add(FoundPOSprite2.Object);
}

void AGloomsTDGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// We create the UI
	if (WidgetClass)
	{
		Widget = CreateWidget<UInGame>(GetWorld()->GetFirstPlayerController(), WidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
			Widget->GameMode = this;
		}
	}
}

void AGloomsTDGameModeBase::ShowMainMenu()
{
	// If there is not another menu visible, we make this menu visible
	if (Widget)
		if (!Widget->IsAnotherMenuOpen)
		{
			Widget->MainMenuVisible();
			Widget->IsAnotherMenuOpen = true;
		}
}

void AGloomsTDGameModeBase::BuyTower(int Tower)
{
	// We calculate the price based on the tower we buy
	switch (Tower)
	{
	case 1:
		Money -= Widget->CalculatePrice(20);
		break;

	case 2:
		Money -= Widget->CalculatePrice(25);
		break;

	case 3:
		Money -= Widget->CalculatePrice(25);
		break;

	case 4:
		Money -= Widget->CalculatePrice(50);
		break;
	}

	// We increase the number of towers and we enter in the building mode
	Towers++;
	BuildingMode(Tower);
	PlayerObjective->BuildMode = true;
}

void AGloomsTDGameModeBase::BuildingMode(int Tower)
{
	// We spawn the tower that we are building and pass it to Player Objective
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	switch (Tower)
	{
	case 1:
		PlayerObjective->BuildTower = GetWorld()->SpawnActor<ASPE>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		break;

	case 2:
		PlayerObjective->BuildTower = GetWorld()->SpawnActor<AMirror>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		break;

	case 3:
		PlayerObjective->BuildTower = GetWorld()->SpawnActor<APrism>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		break;

	case 4:
		PlayerObjective->BuildTower = GetWorld()->SpawnActor<AEMP>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		break;
	}
}

void AGloomsTDGameModeBase::ShowPOMenu()
{
	// If there is not another menu visible, we make this menu visible
	if (Widget)
		if (!Widget->IsAnotherMenuOpen)
		{
			Widget->POMenuVisible();
			Widget->IsAnotherMenuOpen = true;
		}
}

void AGloomsTDGameModeBase::SetPODamage(int Tier)
{
	// We uodated Player Objective's damage and color based on tier
	PlayerObjective->DamageTier = Tier;
	switch (Tier)
	{
		case 1:
		{
			PlayerObjective->Glow->SetSpriteColor(FColor(255, 0, 0, 255));
			break;
		}

		case 2:
		{
			PlayerObjective->Glow->SetSpriteColor(FColor(255, 255, 0, 255));
			break;
		}

		case 3:
		{
			PlayerObjective->Glow->SetSpriteColor(FColor(255, 255, 255, 255));
			break;
		}
	}

	// Laser needs to be reinitialized to update
	if (PlayerObjective->WeaponTier == 3)
		PlayerObjective->ShootLaser();
}

void AGloomsTDGameModeBase::SetPODirection(int Tier)
{
	// We uodated Player Objective's direction and color based on tier
	PlayerObjective->DirectionTier = Tier;
	switch (Tier)
	{
		case 1:
		{
			PlayerObjective->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
			break;
		}

		case 2:
		{
			PlayerObjective->Sprite->SetSpriteColor(FColor(255, 255, 0, 255));
			break;
		}

		case 3:
		{
			PlayerObjective->Sprite->SetSpriteColor(FColor(255, 255, 255, 255));
			break;
		}
	}

	// Laser needs to be reinitialized to update
	if (PlayerObjective->WeaponTier == 3)
		PlayerObjective->ShootLaser();
}

void AGloomsTDGameModeBase::SetPOTower(int Tier)
{
	// We set the projectile tier, equivalent sprite and glow size
	PlayerObjective->WeaponTier = Tier;
	PlayerObjective->Sprite->SetSprite(POSprite[Tier - 1]);
	PlayerObjective->Glow->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));

	// We change the shooting timer to the proper one
	if (Tier == 2)
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerObjective->ShootHandle);
		GetWorld()->GetTimerManager().SetTimer(PlayerObjective->ShootHandle, PlayerObjective, &APlayerObjective::ShootingTimer, 0.55f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerObjective->ShootHandle);
		PlayerObjective->ShootLaser();
	}
}

void AGloomsTDGameModeBase::ShowTowerMenu(ATower * Tower)
{
	// If there is not another menu visible, we make this menu visible
	if(Widget)
		if (!Widget->IsAnotherMenuOpen)
		{
			Widget->Tower = Tower;
			Widget->TowerVisible();
			Widget->IsAnotherMenuOpen = true;
		}
}

void AGloomsTDGameModeBase::EndWave()
{
	// We increase the number that keeps count of how many splines ended their waves
	EndedWaves++;
	// If all the waves ended their waves, we reset EndedWaves to 0 and increase the wave count on each wave
	if (EndedWaves == Spline.Num())
	{
		EndedWaves = 0;
		for (int i = 0; i < Spline.Num(); i++)
			Spline[i]->Wave++;
		// We tell the UI there no longer is a wave in progress
		Widget->IsWaveInProgress(false);
	}
}

void AGloomsTDGameModeBase::LoseLife()
{
	// We decrease the number of lives and we check if the player still has lives left
	Lives--;
	if (Lives == 0)
		Widget->ShowGameOver();
}
