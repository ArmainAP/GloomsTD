// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "Mirror.generated.h"

UCLASS()
class GLOOMSTD_API AMirror : public ATower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMirror();
	// Called on tower upgrade
	virtual void UpdateSprite(int Tier) override;
	// Called upon collision with a photon projectile
	virtual void Photon(FRotator Rotation) override;
	// Called upon collision with a wave projectile
	virtual void Wave(FRotator Rotation) override;
	// Called upon collision with a laser projectile
	virtual void Laser(FRotator Rotation);
	// Called to shoot a photon
	void ShootPhoton(FRotator Rotation);
	// Called to shoot a wave
	void ShootWave(FRotator Rotation);
	// Called to shoot a laser
	void ShootLaser(FRotator Rotation);
	// Called to clear the existing lasers.
	void ClearLaser();

public:
	FTimerHandle DestroyLaser;
	bool bLaser;
};
