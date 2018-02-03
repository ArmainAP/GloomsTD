// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"

AMirror::AMirror()
{
	// We set the base buy price for the tower
	BasePrice = 25;

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite0(TEXT("/Game/Assets/Towers/Mirror1_Sprite"));
	if (FoundSprite0.Succeeded())
		TowerSprite.Add(FoundSprite0.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite1(TEXT("/Game/Assets/Towers/Mirror2_Sprite"));
	if (FoundSprite1.Succeeded())
		TowerSprite.Add(FoundSprite1.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite2(TEXT("/Game/Assets/Towers/Mirror3_Sprite"));
	if (FoundSprite2.Succeeded())
		TowerSprite.Add(FoundSprite2.Object);

	// We update the game sprite for Tier 1
	UpdateSprite(1);
}

void AMirror::UpdateSprite(int Tier)
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

void AMirror::Photon(FRotator Rotation)
{
	Super::Photon(Rotation);

	// We reverse the dirrection of the projectile
	Rotation += FRotator(0.0f, 180.0f, 0.0f);

	switch (Tier)
	{
	case 1: // We apply a random deviation of 60 degrees and we shoot back the photon
		Rotation += (FMath::RandBool()) ? FRotator(0.0f, 60.0f, 0.0f) : FRotator(0.0f, -60.0f, 0.0f);
		ShootPhoton(Rotation);
		break;

	case 2: // We divide the damage and we shoot 2 projectiles with a deviation of 45 degrees
		Damage = FMath::CeilToInt((float)Damage / 2);
		ShootPhoton(Rotation + FRotator(0.0f, 45.0f, 0.0f));
		ShootPhoton(Rotation - FRotator(0.0f, 45.0f, 0.0f));
		break;

	case 3: // We divide the damage and we shoot 4 projectiles with a deviation of 30 and 60 degrees
		Damage = FMath::CeilToInt((float)Damage / 4);
		ShootPhoton(Rotation + FRotator(0.0f, 60.0f, 0.0f));
		ShootPhoton(Rotation + FRotator(0.0f, 30.0f, 0.0f));
		ShootPhoton(Rotation - FRotator(0.0f, 30.0f, 0.0f));
		ShootPhoton(Rotation - FRotator(0.0f, 60.0f, 0.0f));
		break;
	}
}

void AMirror::Wave(FRotator Rotation)
{
	Super::Wave(Rotation);

	// We reverse the dirrection of the projectile
	Rotation += FRotator(0.0f, 180.0f, 0.0f);

	switch (Tier)
	{
	case 1: // We apply a random deviation of 60 degrees and we shoot back the photon
		Rotation += (FMath::RandBool()) ? FRotator(0.0f, 60.0f, 0.0f) : FRotator(0.0f, -60.0f, 0.0f);
		ShootWave(Rotation);
		break;

	case 2: // We divide the damage and we shoot 2 projectiles with a deviation of 45 degrees
		Damage = FMath::CeilToInt((float)Damage / 2);
		ShootWave(Rotation + FRotator(0.0f, 45.0f, 0.0f));
		ShootWave(Rotation - FRotator(0.0f, 45.0f, 0.0f));
		break;

	case 3: // We divide the damage and we shoot 4 projectiles with a deviation of 30 and 60 degrees
		Damage = FMath::CeilToInt((float)Damage / 4);
		ShootWave(Rotation + FRotator(0.0f, 60.0f, 0.0f));
		ShootWave(Rotation + FRotator(0.0f, 30.0f, 0.0f));
		ShootWave(Rotation - FRotator(0.0f, 30.0f, 0.0f));
		ShootWave(Rotation - FRotator(0.0f, 60.0f, 0.0f));
		break;
	}
}

void AMirror::Laser(FRotator Rotation)
{
	Super::Laser(Rotation);

	// We know that the tower is being hit by a laser
	bLaser = true;

	// We reverse the dirrection of the projectile
	Rotation += FRotator(0.0f, 180.0f, 0.0f);

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
			LaserChild[0]->SetActorRotation(Rotation + FRotator(0.0f, 60.0f, 0.0f));
			LaserChild[1]->SetActorRotation(Rotation + FRotator(0.0f, 30.0f, 0.0f));
			LaserChild[2]->SetActorRotation(Rotation - FRotator(0.0f, 30.0f, 0.0f));
			LaserChild[3]->SetActorRotation(Rotation - FRotator(0.0f, 60.0f, 0.0f));
			break;

		case 3:
			LaserChild[0]->SetActorRotation(Rotation + FRotator(0.0f, 90.0f, 0.0f));
			LaserChild[1]->SetActorRotation(Rotation + FRotator(0.0f, 60.0f, 0.0f));
			LaserChild[2]->SetActorRotation(Rotation + FRotator(0.0f, 30.0f, 0.0f));
			LaserChild[3]->SetActorRotation(Rotation - FRotator(0.0f, 30.0f, 0.0f));
			LaserChild[4]->SetActorRotation(Rotation - FRotator(0.0f, 60.0f, 0.0f));
			LaserChild[5]->SetActorRotation(Rotation - FRotator(0.0f, 90.0f, 0.0f));
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

		case 2: // We divide the damage and we shoot 4 lasers with a deviation of 30 and 60 degrees
			Damage = FMath::CeilToInt((float)Damage / 4);
			ShootLaser(Rotation + FRotator(0.0f, 60.0f, 0.0f));
			ShootLaser(Rotation + FRotator(0.0f, 30.0f, 0.0f));
			ShootLaser(Rotation - FRotator(0.0f, 30.0f, 0.0f));
			ShootLaser(Rotation - FRotator(0.0f, 60.0f, 0.0f));
			break;

		case 3: // We divide the damage and we shoot 6 lasers with a deviation of 30, 60 and 90 degrees
			Damage = FMath::CeilToInt((float)Damage / 6);
			ShootLaser(Rotation + FRotator(0.0f, 90.0f, 0.0f));
			ShootLaser(Rotation + FRotator(0.0f, 60.0f, 0.0f));
			ShootLaser(Rotation + FRotator(0.0f, 30.0f, 0.0f));
			ShootLaser(Rotation - FRotator(0.0f, 30.0f, 0.0f));
			ShootLaser(Rotation - FRotator(0.0f, 60.0f, 0.0f));
			ShootLaser(Rotation - FRotator(0.0f, 90.0f, 0.0f));
			break;
		}

		// We set a timer to check if the tower is still targeted by the laser
		GetWorld()->GetTimerManager().SetTimer(DestroyLaser, this, &AMirror::ClearLaser, 0.1f, true);
	}
}

void AMirror::ShootPhoton(FRotator Rotation)
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

void AMirror::ShootWave(FRotator Rotation)
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

void AMirror::ShootLaser(FRotator Rotation)
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

void AMirror::ClearLaser()
{
	// If the tower is target by a lasr and we have spawned laser children, we clear those and the timer
	if(!bLaser)
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
