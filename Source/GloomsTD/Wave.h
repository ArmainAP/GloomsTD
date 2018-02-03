// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Wave.generated.h"

UCLASS()
class GLOOMSTD_API AWave : public AProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWave();
	// Called upon construction in editor
	virtual void OnConstruction(const FTransform& transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the actor overlaps another actor
	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	float InnerRadius;

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
		UMaterialInterface* WaveMaterial;
};
