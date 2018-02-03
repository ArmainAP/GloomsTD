// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "EMP.generated.h"

/**
 * 
 */

class AEMPWave;

UCLASS()
class GLOOMSTD_API AEMP : public ATower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEMP();
	// Called upon construction in editor
	virtual void OnConstruction(const FTransform & transform) override;
	// Called on tower upgrade
	virtual void UpdateSprite(int Tier) override;
	// Called upon collision with a photon projectile
	virtual void Photon(FRotator Rotator) override;
	// Called upon collision with a wave projectile
	virtual void Wave(FRotator Rotator) override;
	// Called upon collision with a laser projectile
	virtual void Laser(FRotator Rotation) override;
	// Called when we allow the tower to shoot
	virtual void CanShootOn() override;
	// Called to create the emp wave or to grow the emp wave
	void EMPWave();

public:
	bool bLaser;

	UPROPERTY()
		AEMPWave* Child;

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
		UMaterialInterface* GlowMaterial;
};