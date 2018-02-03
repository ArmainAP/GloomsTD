// Fill out your copyright notice in the Description page of Project Settings.

#include "EMPWave.h"
#include "EMP.h"
#include "Gloom.h"

// Sets default values
AEMPWave::AEMPWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the component that will handle the collision
	// We disable collision on it and enable it for the sprite to use sprite's bounds for collision instead of a sphere.
	this->SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	this->RootComponent = SphereComponent;

	// This is the component that renders the sprite
	this->Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
	Sprite->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Player/EMPWaveSprite"));
	if (FoundSprite.Succeeded())
		Sprite->SetSprite(FoundSprite.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial (TEXT("/Game/Assets/Player/EMPWave_Inst"));
	if (FoundMaterial.Succeeded())
		SpriteMaterial = FoundMaterial.Object;
}

void AEMPWave::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	DynamicMaterial = UMaterialInstanceDynamic::Create(SpriteMaterial, this);
	DynamicMaterial->SetScalarParameterValue("InnerRadius", InnerRadius);
	Sprite->SetMaterial(0, DynamicMaterial);

	// We set the sprite color to black
	Sprite->SetSpriteColor(FColor(0, 0, 0, 255));
}

void AEMPWave::BeginPlay()
{
	Super::BeginPlay();

	// We use the AddDynamic macro to create a delegate for collision
	OnActorBeginOverlap.AddDynamic(this, &AEMPWave::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AEMPWave::EndOverlap);
}

void AEMPWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the projectile is not allowed to shrink and it did not reach the max size yet, we grow it.
	if (!CanShrink && GetActorScale3D().X <= 3.0f)
		SetActorScale3D(GetActorScale3D() + 5 * DeltaTime);

	// If the actor can shrink we reduce the inner radius of the projectile
	if (CanShrink)
	{
		InnerRadius += DeltaTime * 0.1;
		DynamicMaterial->SetScalarParameterValue("InnerRadius", InnerRadius);
	}

	// If the inner radius has a value that makes the sprite invisible from the player, we destroy the projectile
	if (InnerRadius >= 0.5f)
	{
		Parent->Child = NULL;
		Destroy();
	}
}

void AEMPWave::Refuel()
{
	// We reset the inner radius to avoid deletion
	InnerRadius = 0.4f;
	DynamicMaterial->SetScalarParameterValue("InnerRadius", 0.4f);
}

/*
 * Because I had no way to check for collision inside the projectile,
 * I found a way to deal with it around by dealing the damage twice,
 * once when the glooms collide with the projectile and once when the exit the projectile.
 * NOTE! This sprite uses areas with the alpha 0 to create the ring effect.
 */
void AEMPWave::BeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
		Gloom->DirectDamage(Damage);
}

void AEMPWave::EndOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
		Gloom->DirectDamage(Damage);
}
