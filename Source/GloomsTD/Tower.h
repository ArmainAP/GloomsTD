// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Projectile.h"
#include "Photon.h"
#include "Wave.h"
#include "LASER.h"
#include "EMPWave.h"
#include "SPEPhoton.h"
#include "Tower.generated.h"

UCLASS()
class GLOOMSTD_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	// Called on tower upgrade
	virtual void UpdateSprite(int Tier);
	// Called upon collision with a photon projectile
	virtual void Photon(FRotator Rotation);
	// Called upon collision with a wave projectile
	virtual void Wave(FRotator Rotation);
	// Called upon collision with a laser projectile
	virtual void Laser(FRotator Rotation);
	// Called when we allow the tower to shoot
	virtual void CanShootOn();
	// Called when we want to calculate the upgrade price
	void CalculateUpgradePrice();

public:	
	int BasePrice;
	bool CanPlace;
	bool CanShoot = true;
	bool IsInBuildinMode = true;

	UPROPERTY()
		USphereComponent* SphereComponent;

	UPROPERTY()
		UPaperSpriteComponent* Sprite;

	UPROPERTY()
		UPaperSpriteComponent* Glow;

	UPROPERTY()
		TArray<ALASER*> LaserChild;

	UPROPERTY()
		TArray<UPaperSprite*> TowerSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Tier = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int UpgradePrice;
};
