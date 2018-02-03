// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "LASER.generated.h"

UCLASS()
class GLOOMSTD_API ALASER : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALASER();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to set the laser's size in lenght 
	void SetBeam();

	// Called when the actor overlaps another actor
	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FRotator Offset;
	FVector EndPoint;
	int Damage;

	UPROPERTY()
		USceneComponent* SceneComponent;

	UPROPERTY()
		UBoxComponent* BoxComponent;

	UPROPERTY()
		UParticleSystemComponent* Particle;

	UPROPERTY()
		AActor* Parent;
};