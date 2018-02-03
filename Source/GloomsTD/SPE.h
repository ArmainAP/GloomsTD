// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "SPE.generated.h"

class AGloom;

UCLASS()
class GLOOMSTD_API ASPE : public ATower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPE();
	virtual void OnConstruction(const FTransform & transform) override;
	// Called on tower upgrade
	virtual void UpdateSprite(int Tier) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called upon collision with a photon projectile
	virtual void Photon(FRotator Rotation) override;
	// Called upon collision with a wave projectile
	virtual void Wave(FRotator Rotation) override;
	// Called upon collision with a laser projectile
	virtual void Laser(FRotator Rotation) override;
	// Called to shoot a projectile
	void Shoot();

	// Called when the actor overlaps another actor
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the actor no longer overlaps another actor
	UFUNCTION() 
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
		TArray<AGloom*> Glooms;

	UPROPERTY()
		UMaterialInterface* GlowMaterial;
};
