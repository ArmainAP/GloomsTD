// Fill out your copyright notice in the Description page of Project Settings.

#include "Gloom.h"
#include "SplinePath.h"
#include "GloomsTDGameModeBase.h"

// Sets default values
AGloom::AGloom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the component that will handle the collision
	this->SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetSphereRadius(9.0f);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	this->RootComponent = SphereComponent;

	// This is the component that renders the core of the enemy
	this->GloomCore = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Core"));
	GloomCore->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
	GloomCore->SetMobility(EComponentMobility::Movable);
	GloomCore->SetCollisionProfileName(TEXT("NoCollision"));
	GloomCore->SetupAttachment(RootComponent);

	// This is the component that renders the shield of the enemy
	this->GloomShield = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Shield"));
	GloomShield->SetWorldLocation(FVector(0.0f, -1.0f, 0.0f));
	GloomShield->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	GloomShield->SetMobility(EComponentMobility::Movable);
	GloomShield->SetCollisionProfileName(TEXT("NoCollision"));
	GloomShield->SetupAttachment(GloomCore);

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Gloom/Gloom_Spr"));
	if (FoundSprite.Succeeded())
	{
		GloomCore->SetSprite(FoundSprite.Object);
		GloomShield->SetSprite(FoundSprite.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundGloomCoreMaterial(TEXT("/Game/Assets/Gloom/Health"));
	if (FoundGloomCoreMaterial.Succeeded())
		GloomCore->SetMaterial(0, FoundGloomCoreMaterial.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundGloomShieldMaterial(TEXT("/Game/Assets/Gloom/Shield"));
	if (FoundGloomShieldMaterial.Succeeded())
		ShieldMaterial = FoundGloomShieldMaterial.Object;
}

void AGloom::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ShieldMaterial, this);
	DynamicMaterial->SetScalarParameterValue("Radius", 0.75f);
	GloomShield->SetMaterial(0, DynamicMaterial);
}

void AGloom::PostSpawn(ASplinePath * SplinePath, FColor Color)
{
	// We store a reference for the spline path and the color of the enemy
	Path = SplinePath;
	SpriteColor = Color;

	// We set the color of the enemy
	GloomCore->SetSpriteColor(SpriteColor);
	GloomShield->SetSpriteColor(SpriteColor);

	// We set the speed multiplier based on the color
	Speed = float(SpriteColor.G) / 100;
}

// Called every frame
void AGloom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We move it forward to the end of the spline
	DistanceAlongSpline += 100 * DeltaTime + Speed * 100 * DeltaTime;
	SetActorLocation(FVector(Path->Spline->GetWorldLocationAtDistanceAlongSpline(DistanceAlongSpline).X, Path->Spline->GetWorldLocationAtDistanceAlongSpline(DistanceAlongSpline).Y, 2.0f));
	
	// We enable collision when the gloom is no longer off screen
	// We avoid spawn kills with lasers using this
	if ((GetActorLocation().X < 640) && (GetActorLocation().X > -640) && (GetActorLocation().Y < 360) && (GetActorLocation().Y > -360))
		SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	// When the nemy reaches the end of the spline, we kill it with no rewards and we take a life from the player.
	if (Path->Spline->GetSplineLength() < DistanceAlongSpline)
	{
		Path->EnemiesToKill--;
		Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->LoseLife();
		Destroy();
	}
}

void AGloom::Damage(int Damage)
{
	// Health and Shield are based on colors, to work easier with them we save them in variables
	int Health = SpriteColor.R;
	int Shield = SpriteColor.B;

	// We deal damage to the shield
	Shield = Shield - Damage;
	// If the damage ammount was bigger than the shield ammount, we deal the remaining ammout to the life
	if (Shield < 0)
	{
		Health = Health + Shield;
		Shield = 0;
	}

	// NOTE! Having Health == 0 means it can sustain one hit
	// If the health is smaller than 0 we award the player with money and destroy the enemy
	if (Health < 0)
	{
		Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->Money += SpriteColor.A;
		Destroy();
	}
	// Else, the enemy is still alive and we update the sprite color
	else
	{
		SpriteColor = FColor(Health, SpriteColor.G, Shield, SpriteColor.A);
		GloomCore->SetSpriteColor(SpriteColor);
		GloomShield->SetSpriteColor(SpriteColor);
	}
}

// Same as Damage(int Damage) but it ignores the shield
void AGloom::DirectDamage(int Damage)
{
	int Health = SpriteColor.R;
	Health = Health - Damage;
	if (Health < 0)
	{
		Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->Money += SpriteColor.A;
		Destroy();
	}
	else
	{
		SpriteColor = FColor(Health, SpriteColor.G, SpriteColor.B, SpriteColor.A);
		GloomCore->SetSpriteColor(SpriteColor);
		GloomShield->SetSpriteColor(SpriteColor);
	}
}

void AGloom::Destroyed()
{
	Super::Destroyed();

	// When the enemy dies we tell the SplinePath to increase the number of enemies killed in that wave
	if (Path)
		Path->EnemiesKilled++;
}