// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Component headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Gloom.generated.h"

class ASplinePath;

UCLASS()
class GLOOMSTD_API AGloom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGloom();
	// Called upon construction in editor
	virtual void OnConstruction(const FTransform& transform) override;
	// Callled every frame
	virtual void Tick(float DeltaTime) override;
	// Called when deleted
	virtual void Destroyed() override;
	// Called when it takes damage
	void Damage(int Damage);
	// Called when it takes damage directly to the health, ignoring the shield
	void DirectDamage(int Damage);
	// Called after spawn
	void PostSpawn(ASplinePath* SplinePath, FColor Color);

public:
	float DistanceAlongSpline;
	float Speed;
	FColor SpriteColor = FColor(0, 0, 0, 1);

	UPROPERTY()
		USphereComponent* SphereComponent;

	UPROPERTY()
		UPaperSpriteComponent* GloomCore;

	UPROPERTY()
		UPaperSpriteComponent* GloomShield;

	UPROPERTY()
		ASplinePath* Path;

	UPROPERTY() //We avoid Garbage Collection picking up this variable with UPROPERTY()
		UMaterialInterface* ShieldMaterial;
};
