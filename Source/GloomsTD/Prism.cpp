// Fill out your copyright notice in the Description page of Project Settings.

#include "Prism.h"
#include "GloomsTDGameModeBase.h"

APrism::APrism()
{
	// We set the base buy price for the tower
	BasePrice = 25;

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite0(TEXT("/Game/Assets/Towers/Prism1_Sprite"));
	if (FoundSprite0.Succeeded())
		TowerSprite.Add(FoundSprite0.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite1(TEXT("/Game/Assets/Towers/Prism2_Sprite"));
	if (FoundSprite1.Succeeded())
		TowerSprite.Add(FoundSprite1.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite2(TEXT("/Game/Assets/Towers/Prism3_Sprite"));
	if (FoundSprite2.Succeeded())
		TowerSprite.Add(FoundSprite2.Object);

	// We update the game sprite for Tier 1
	UpdateSprite(1);
}

void APrism::UpdateSprite(int Tier)
{
	Super::UpdateSprite(Tier);

	// We set the equivalent sprite for the tier
	Sprite->SetSprite(TowerSprite[Tier - 1]);

	// We destroy the lasers because they will need to be reinitialized 
	if (LaserChild.Num() > 0)
	{
		for (int i = 0; i < LaserChild.Num(); i++)
			LaserChild[i]->Destroy();
		LaserChild.Empty();
	}
}

void APrism::Photon(FRotator Rotation)
{
	Super::Photon(Rotation);

	switch (Tier)
	{
		case 1: // We apply a random deviation of 60 degrees and we shoot the projectile
			Rotation += (FMath::RandBool()) ? FRotator(0.0f, 60.0f, 0.0f) : FRotator(0.0f, -60.0f, 0.0f);
			ShootPhoton(Rotation);
			break;

		case 2: // We apply a deviation of 60 degrees and shoot 3 projectiles
			ShootColoredPhoton(Rotation - FRotator(0.0f, 60, 0.0f), 0);
			ShootColoredPhoton(Rotation, 1);
			ShootColoredPhoton(Rotation + FRotator(0.0f, 60, 0.0f), 2);
			break;

		case 3: // We apply a deviation of 30, 60, 90 degrees and shoot 7 projectiles
			ShootColoredPhoton(Rotation - FRotator(0.0f, 90, 0.0f), 0);
			ShootColoredPhoton(Rotation - FRotator(0.0f, 60, 0.0f), 1);
			ShootColoredPhoton(Rotation - FRotator(0.0f, 30, 0.0f), 2);
			ShootColoredPhoton(Rotation, 3);
			ShootColoredPhoton(Rotation + FRotator(0.0f, 30, 0.0f), 4);
			ShootColoredPhoton(Rotation + FRotator(0.0f, 60, 0.0f), 5);
			ShootColoredPhoton(Rotation + FRotator(0.0f, 90, 0.0f), 6);
			break;
	}
}

void APrism::Wave(FRotator Rotation)
{
	Super::Wave(Rotation);

	switch (Tier)
	{
	case 1: // We apply a random deviation of 60 degrees and we shoot the projectile
		Rotation += (FMath::RandBool()) ? FRotator(0.0f, 60.0f, 0.0f) : FRotator(0.0f, -60.0f, 0.0f);
		ShootWave(Rotation);
		break;

	case 2: // We apply a deviation of 60 degrees and shoot 3 projectiles
		ShootColoredWave(Rotation - FRotator(0.0f, 60, 0.0f), 0);
		ShootColoredWave(Rotation, 1);
		ShootColoredWave(Rotation + FRotator(0.0f, 60, 0.0f), 2);
		break;

	case 3: // We apply a deviation of 30, 60, 90 degrees and shoot 7 projectiles
		ShootColoredWave(Rotation - FRotator(0.0f, 90, 0.0f), 0);
		ShootColoredWave(Rotation - FRotator(0.0f, 60, 0.0f), 1);
		ShootColoredWave(Rotation - FRotator(0.0f, 30, 0.0f), 2);
		ShootColoredWave(Rotation, 3);
		ShootColoredWave(Rotation + FRotator(0.0f, 30, 0.0f), 4);
		ShootColoredWave(Rotation + FRotator(0.0f, 60, 0.0f), 5);
		ShootColoredWave(Rotation + FRotator(0.0f, 90, 0.0f), 6);
		break;
	}
}

void APrism::Laser(FRotator Rotation)
{
	Super::Laser(Rotation);

	// We know that the tower is being hit by a laser
	bLaser = true;

	// If there are any spawned lasers, all we need to do is to update the rotation
	if (LaserChild.Num() > 0)
	{
		switch (Tier)
		{
		case 1:
			LaserChild[0]->SetActorRotation(Rotation + FRotator(0.0f, 45.0f, 0.0f));
			LaserChild[1]->SetActorRotation(Rotation - FRotator(0.0f, 45.0f, 0.0f));
			break;

		case 2:
			LaserChild[0]->SetActorRotation(Rotation - FRotator(0.0f, 60.0f, 0.0f));
			LaserChild[1]->SetActorRotation(Rotation);
			LaserChild[2]->SetActorRotation(Rotation + FRotator(0.0f, 60.0f, 0.0f));
			break;

		case 3:
			LaserChild[0]->SetActorRotation(Rotation - FRotator(0.0f, 90.0f, 0.0f));
			LaserChild[1]->SetActorRotation(Rotation - FRotator(0.0f, 60.0f, 0.0f));
			LaserChild[2]->SetActorRotation(Rotation - FRotator(0.0f, 30.0f, 0.0f));
			LaserChild[3]->SetActorRotation(Rotation);
			LaserChild[4]->SetActorRotation(Rotation + FRotator(0.0f, 30.0f, 0.0f));
			LaserChild[5]->SetActorRotation(Rotation + FRotator(0.0f, 60.0f, 0.0f));
			LaserChild[6]->SetActorRotation(Rotation + FRotator(0.0f, 90.0f, 0.0f));
			break;
		}
	}
	else
	{
		switch (Tier)
		{
		case 1: // We divide the damage and we shoot 2 lasers with a deviation of 45 degrees
			Damage = FMath::CeilToInt((float)Damage / 2);
			ShootLaser(Rotation + FRotator(0.0f, 45.0f, 0.0f));
			ShootLaser(Rotation - FRotator(0.0f, 45.0f, 0.0f));
			break;

		case 2: // We shoot 3 lasers with a deviation of 60 degrees
			ShootColoredLaser(Rotation - FRotator(0.0f, 60, 0.0f), 0);
			ShootColoredLaser(Rotation, 1);
			ShootColoredLaser(Rotation + FRotator(0.0f, 60, 0.0f), 2);
			break;

		case 3: // We shoot 7 lasers with a deviation of 30, 60 and 90 degrees
			ShootColoredLaser(Rotation - FRotator(0.0f, 90, 0.0f), 0);
			ShootColoredLaser(Rotation - FRotator(0.0f, 60, 0.0f), 1);
			ShootColoredLaser(Rotation - FRotator(0.0f, 30, 0.0f), 2);
			ShootColoredLaser(Rotation, 3);
			ShootColoredLaser(Rotation + FRotator(0.0f, 30, 0.0f), 4);
			ShootColoredLaser(Rotation + FRotator(0.0f, 60, 0.0f), 5);
			ShootColoredLaser(Rotation + FRotator(0.0f, 90, 0.0f), 6);
			break;
		}

		// We set a timer to check if the tower is still targeted by the laser
		GetWorld()->GetTimerManager().SetTimer(DestroyLaser, this, &APrism::ClearLaser, 0.1f, true);
	}
}

void APrism::ShootPhoton(FRotator Rotation)
{
	// We spawn the projectile and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APhoton* SpawnedPhoton = GetWorld()->SpawnActor<APhoton>(Location, Rotation, SpawnInfo);
	if (SpawnedPhoton)
	{
		SpawnedPhoton->Parent = this;
		SpawnedPhoton->CanOverlap = true;
		SpawnedPhoton->Sprite->SetSpriteColor(Sprite->GetSpriteColor());
		SpawnedPhoton->Damage = Damage;
	}
}

void APrism::ShootColoredPhoton(FRotator Rotation, int PhotonNumber)
{
	// We spawn the projectile and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APhoton* SpawnedPhoton = GetWorld()->SpawnActor<APhoton>(Location, Rotation, SpawnInfo);
	if (SpawnedPhoton)
	{
		SpawnedPhoton->Parent = this;
		SpawnedPhoton->CanOverlap = true;
		// We set the damage and the color for Tier 2
		if (Tier == 2)
		{
			SpawnedPhoton->Damage = FMath::CeilToInt((float)Damage / 3);
			switch (PhotonNumber)
			{
			case 0:
				//Red
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
				break;

			case 1:
				//Green
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(0, 255, 0, 255));
				break;

			case 2:
				//Blue
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(0, 0, 255, 255));
				break;
			}
		}
		// We set the damage and the color for Tier 3
		else
		{
			switch (PhotonNumber)
			{
			case 0:
				//Red
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt((float)Damage / 3);
				break;

			case 1:
				//Orange
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 128, 0, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt(((float)Damage / 3) + ((float)Damage / 6));
				break;

			case 2:
				//Yellow
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 255, 0, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt((float)Damage * 2 / 3);
				break;

			case 3:
				//Green
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(0, 255, 0, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt((float)Damage / 3);
				break;

			case 4:
				//Blue
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(0, 0, 255, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt((float)Damage / 3);
				break;

			case 5:
				//Indigo
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(75, 0, 130, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt((float)Damage * 2 / 3);
				break;

			case 6:
				//Violet
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(128, 0, 255, 255));
				SpawnedPhoton->Damage = FMath::CeilToInt(((float)Damage / 3) + ((float)Damage / 6));
				break;
			}
		}
	}
}

void APrism::ShootWave(FRotator Rotation)
{
	// We spawn the projectile and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AWave* SpawnedWave = GetWorld()->SpawnActor<AWave>(Location, Rotation, SpawnInfo);
	if (SpawnedWave)
	{
		SpawnedWave->Parent = this;
		SpawnedWave->CanOverlap = true;
		SpawnedWave->Sprite->SetSpriteColor(Sprite->GetSpriteColor());
		SpawnedWave->Damage = Damage;
	}
}

void APrism::ShootColoredWave(FRotator Rotation, int WaveNumber)
{
	// We spawn the projectile and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AWave* SpawnedWave = GetWorld()->SpawnActor<AWave>(Location, Rotation, SpawnInfo);
	if (SpawnedWave)
	{
		SpawnedWave->Parent = this;
		SpawnedWave->CanOverlap = true;
		// We set the damage and the color for Tier 2
		if (Tier == 2)
		{
			SpawnedWave->Damage = FMath::CeilToInt((float)Damage / 3);
			switch (WaveNumber)
			{
			case 0:
				//Red
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
				break;

			case 1:
				//Green
				SpawnedWave->Sprite->SetSpriteColor(FColor(0, 255, 0, 255));
				break;

			case 2:
				//Blue
				SpawnedWave->Sprite->SetSpriteColor(FColor(0, 0, 255, 255));
				break;
			}
		}
		// We set the damage and the color for Tier 3
		else
		{
			switch (WaveNumber)
			{
			case 0:
				//Red
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
				SpawnedWave->Damage = FMath::CeilToInt((float)Damage / 3);
				break;

			case 1:
				//Orange
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 128, 0, 255));
				SpawnedWave->Damage = FMath::CeilToInt(((float)Damage / 3) + ((float)Damage / 6));
				break;

			case 2:
				//Yellow
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 255, 0, 255));
				SpawnedWave->Damage = FMath::CeilToInt((float)Damage * 2 / 3);
				break;

			case 3:
				//Green
				SpawnedWave->Sprite->SetSpriteColor(FColor(0, 255, 0, 255));
				SpawnedWave->Damage = FMath::CeilToInt((float)Damage / 3);
				break;

			case 4:
				//Blue
				SpawnedWave->Sprite->SetSpriteColor(FColor(0, 0, 255, 255));
				SpawnedWave->Damage = FMath::CeilToInt((float)Damage / 3);
				break;

			case 5:
				//Indigo
				SpawnedWave->Sprite->SetSpriteColor(FColor(75, 0, 130, 255));
				SpawnedWave->Damage = FMath::CeilToInt((float)Damage * 2 / 3);
				break;

			case 6:
				//Violet
				SpawnedWave->Sprite->SetSpriteColor(FColor(128, 0, 255, 255));
				SpawnedWave->Damage = FMath::CeilToInt(((float)Damage / 3) + ((float)Damage / 6));
				break;
			}
		}
	}
}

void APrism::ShootLaser(FRotator Rotation)
{
	// We spawn the laser and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ALASER* SpawnedLaser = GetWorld()->SpawnActor<ALASER>(Location, Rotation, SpawnInfo);
	if (SpawnedLaser)
	{
		LaserChild.Add(SpawnedLaser);
		SpawnedLaser->Parent = this;
		SpawnedLaser->Particle->SetColorParameter("Color", Sprite->GetSpriteColor());
		SpawnedLaser->Damage = Damage;
	}
}

void APrism::ShootColoredLaser(FRotator Rotation, int WaveNumber)
{
	// We spawn the laser and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ALASER* SpawnedLaser = GetWorld()->SpawnActor<ALASER>(Location, Rotation, SpawnInfo);
	if (SpawnedLaser)
	{
		LaserChild.Add(SpawnedLaser);
		SpawnedLaser->Parent = this;
		// We set the damage and the color for Tier 2
		if (Tier == 2)
		{
			SpawnedLaser->Damage = FMath::CeilToInt((float)Damage / 3);
			switch (WaveNumber)
			{
			case 0:
				//Red
				SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 0, 0, 255));
				break;

			case 1:
				//Green
				SpawnedLaser->Particle->SetColorParameter("Color", FColor(0, 255, 0, 255));
				break;

			case 2:
				//Blue
				SpawnedLaser->Particle->SetColorParameter("Color", FColor(0, 0, 255, 255));
				break;
			}
		}
		// We set the damage and the color for Tier 3
		else
		{
			switch (WaveNumber)
		{
		case 0:
			//Red
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 0, 0, 255));
			SpawnedLaser->Damage = FMath::CeilToInt((float)Damage / 3);
			break;

		case 1:
			//Orange
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 128, 0, 255));
			SpawnedLaser->Damage = FMath::CeilToInt(((float)Damage / 3) + ((float)Damage / 6));
			break;

		case 2:
			//Yellow
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 255, 0, 255));
			SpawnedLaser->Damage = FMath::CeilToInt((float)Damage * 2 / 3);
			break;

		case 3:
			//Green
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(0, 255, 0, 255));
			SpawnedLaser->Damage = FMath::CeilToInt((float)Damage / 3);
			break;

		case 4:
			//Blue
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(0, 0, 255, 255));
			SpawnedLaser->Damage = FMath::CeilToInt((float)Damage / 3);
			break;

		case 5:
			//Indigo
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(75, 0, 130, 255));
			SpawnedLaser->Damage = FMath::CeilToInt((float)Damage * 2 / 3);
			break;

		case 6:
			//Violet
			SpawnedLaser->Particle->SetColorParameter("Color", FColor(128, 0, 255, 255));
			SpawnedLaser->Damage = FMath::CeilToInt(((float)Damage / 3) + ((float)Damage / 6));
			break;
		}
		}
	}
}

void APrism::ClearLaser()
{
	// If the tower is target by a lasr and we have spawned laser children, we clear those and the timer
	if (!bLaser)
		if (LaserChild.Num() > 0)
		{
			for (int i = 0; i < LaserChild.Num(); i++)
				LaserChild[i]->Destroy();
			LaserChild.Empty();
			GetWorld()->GetTimerManager().ClearTimer(DestroyLaser);
		}

	// We know that the tower is no longer targeted by a laser
	bLaser = false;
}