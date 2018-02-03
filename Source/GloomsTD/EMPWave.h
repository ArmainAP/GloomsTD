// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "EMPWave.generated.h"

class AEMP;

UCLASS()
class GLOOMSTD_API AEMPWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPWave();
	// Called upon construction in editor
	virtual void OnConstruction(const FTransform & transform) override;
	// Callled every frame
	virtual void Tick(float DeltaTime) override;
	// Called to increase the size of the wave
	void Refuel();

	// Called when the actor overlaps another actor
	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// Called when the actor no longer overlaps another actor
	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	int Damage;
	bool CanShrink;
	float InnerRadius = 0.4f;

	UPROPERTY()
		USphereComponent* SphereComponent;

	UPROPERTY()
		UPaperSpriteComponent* Sprite;

	UPROPERTY()
		AEMP* Parent;

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
		UMaterialInstance* SpriteMaterial;
};
