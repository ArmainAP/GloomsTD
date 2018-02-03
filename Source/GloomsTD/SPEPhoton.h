// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "SPEPhoton.generated.h"

class AGloom;

UCLASS()
class GLOOMSTD_API ASPEPhoton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPEPhoton();
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
	FVector TargetLocation;
	int Damage;

	UPROPERTY()
		USphereComponent* SphereComponent;

	UPROPERTY()
		UPaperSpriteComponent* Sprite;

	UPROPERTY()
		AGloom* Target;

	UPROPERTY()
		UMaterialInterface* PhotonMaterial;
};
