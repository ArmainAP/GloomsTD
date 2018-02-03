// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "Prism.generated.h"

UCLASS()
class GLOOMSTD_API APrism : public ATower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APrism();
	// Called on tower upgrade
	virtual void UpdateSprite(int Tier) override;
	// Called upon collision with a photon projectile
	virtual void Photon(FRotator Rotation) override;
	// Called upon collision with a wave projectile
	virtual void Wave(FRotator Rotation) override;
	// Called upon collision with a laser projectile
	virtual void Laser(FRotator Rotation) override;
	// Called to shoot a photon with the same color as the tower
	void ShootPhoton(FRotator Rotation);
	// Called to shoot a photon with a specific color
	void ShootColoredPhoton(FRotator Rotation, int PhotonNumber);
	// Called to shoot a wave with the same color as the tower
	void ShootWave(FRotator Rotation);
	// Called to shoot a wave with a specific color
	void ShootColoredWave(FRotator Rotation, int WaveNumber);
	// Called to shoot a laser with the same color as the tower
	void ShootLaser(FRotator Rotation);
	// Called to shoot a laser with a specific color
	void ShootColoredLaser(FRotator Rotation, int WaveNumber);
	// Called to clear the existing lasers.
	void ClearLaser();

public:
	FTimerHandle DestroyLaser;
	bool bLaser;
};
