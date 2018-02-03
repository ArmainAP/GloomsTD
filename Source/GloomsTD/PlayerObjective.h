// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "PlayerObjective.generated.h"

class ATower;
class ALASER;

UCLASS()
class GLOOMSTD_API APlayerObjective : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerObjective();
	// Called upon construction in editor
	virtual void OnConstruction(const FTransform& transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind input to functions
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when primary click/touch is pressed
	void ClickPressed();
	// Called when primary click/touch is released
	void ClickReleased();
	// Called at a specific interval to shoot a photon or a wave
	void ShootingTimer();
	// Called to shoot a photon or wave
	void Shoot(FRotator Rotation);
	// Called to shot a laser
	void ShootLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	int WeaponTier = 1;
	int DamageTier = 1;
	int DirectionTier = 1;
	bool BuildMode;
	bool IsClickPressed = false;
	float ClickDownTime;
	FHitResult Hit;
	FRotator Rotation;
	FVector TouchLocation;
	FTimerHandle ShootHandle;
	FColor SpriteColor = FColor(255, 0, 0, 255);

	UPROPERTY()
		USphereComponent* SphereComponent;

	UPROPERTY()
		UPaperSpriteComponent* Sprite;

	UPROPERTY()
		UPaperSpriteComponent* Glow;

	UPROPERTY()
		ATower* BuildTower;

	UPROPERTY()
		TArray<ALASER*> LaserChild;

	UPROPERTY() //We avoid Garbage Collection picking up this variable with UPROPERTY()
		UMaterialInterface* GlowMaterial;
};
