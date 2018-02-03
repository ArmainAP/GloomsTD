// Fill out your copyright notice in the Description page of Project Settings.

#include "EMP.h"
#include "EMPWave.h"

AEMP::AEMP()
{
	// We set the base buy price for the tower
	BasePrice = 50;

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Towers/EMP_Sprite"));
	if (FoundSprite.Succeeded())
	{
		Sprite->SetSprite(FoundSprite.Object);
		Glow->SetSprite(FoundSprite.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Assets/Gloom/Shield"));
	if (FoundMaterial.Succeeded())
		GlowMaterial = FoundMaterial.Object;
}

void AEMP::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	DynamicMaterial = UMaterialInstanceDynamic::Create(GlowMaterial, this);
	DynamicMaterial->SetScalarParameterValue("Radius", 0.2f);
	Glow->SetMaterial(0, DynamicMaterial);

	// We update the game sprite for Tier 1
	UpdateSprite(1);
}

void AEMP::UpdateSprite(int Tier)
{
	Super::UpdateSprite(Tier);

	// We set the glow color based on upgrade tier.
	switch (Tier)
	{
	case 1:
		Glow->SetSpriteColor(FColor(255, 0, 0, 255)); // Red
		break;

	case 2:
		Glow->SetSpriteColor(FColor(255, 255, 0, 255)); // Yellow
		break;

	case 3:
		Glow->SetSpriteColor(FColor(255, 255, 255, 255)); // White
		break;
	}

	// Updates projectiles damage
	if (Child)
		Child->Damage = 2 * Tier * Damage;
}

void AEMP::Photon(FRotator Rotator)
{
	Super::Photon(Rotator);

	// We call the event that handles the projectile
	EMPWave();
}

void AEMP::Wave(FRotator Rotator)
{
	Super::Wave(Rotator);

	// We call the event that handles the projectile
	EMPWave();
}

void AEMP::Laser(FRotator Rotation)
{
	Super::Laser(Rotation);

	// We call the event that handles the projectile, and we no longer allow it to shoot additional ones to avoid duplication.
	CanShoot = false;
	EMPWave();
}

void AEMP::CanShootOn()
{
	Super::CanShootOn();

	// We tell the projectile if it can shrink or not
	if (Child)
		Child->CanShrink = CanShoot;
}

void AEMP::EMPWave()
{
	// If there is a spawned projectile, we tell it to stop from shrinking and we grow it
	if (Child)
	{
		Child->CanShrink = false;
		Child->Refuel();
	}
	// Else we create a new spawned projectile
	else
	{
		FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Child = GetWorld()->SpawnActor<AEMPWave>(Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		// If the spawn succeds we tell the projectile who created it and we set the damage.
		if (Child)
		{
			Child->Parent = this;
			Child->Damage = Tier * 2;
		}
	}
}
