// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Classes/Components/SplineComponent.h"
#include "Classes/Components/SplineMeshComponent.h"
#include "SplinePath.generated.h"

class AGloom;

UCLASS()
class GLOOMSTD_API ASplinePath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplinePath();
	// Called upon construction in editor
	virtual void OnConstruction(const FTransform& transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called upon the start of a new wave
	void StartWave();
	// Called every second to spawn enemies when there is a wave in progress
	void SpawningTimer();
	// Called upon the end of a wave
	void EndWave();
	// Called to spawn a new enemy
	void SpawnGloom(FColor Color);

	/*
		Those functions are used to generate the number of enemies each wave. 
		I did not create the algorithm as it was taken from the source below.
		http://oeis.org/A280998
		The numbers generated are numbers that contain a prime number of "1" in ther binary reflected Gray representation.
	*/
	FString BitsToString(int n);
	int Count(FString String, FString ElementToCount);
	bool IsPrime(int n);
	int Sequence(int n);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	int EnemiesToSpawn;
	int EnemiesKilled;	
	int EnemiesToKill;
	FTimerHandle SpawnHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* Spline;

	UPROPERTY()
		UStaticMesh* StaticMesh;

	UPROPERTY()
		UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Wave = 1;
};