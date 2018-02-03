// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"

UCLASS()
class GLOOMSTD_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	int Damage;
	int Speed;
	bool CanOverlap;

	UPROPERTY()
		USphereComponent* SphereComponent;

	UPROPERTY()
		UPaperSpriteComponent* Sprite;

	UPROPERTY()
		AActor* Parent;
};
