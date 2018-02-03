// Fill out your copyright notice in the Description page of Project Settings.

#include "Wave.h"
#include "PlayerObjective.h"
#include "Gloom.h"
#include "Tower.h"

// Sets default values
AWave::AWave()
{
	// We set the speed multiplier
	Speed = 150;

	// We set the collision profiles
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	Sprite->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Player/WaveSprite"));
	if (FoundSprite.Succeeded())
		Sprite->SetSprite(FoundSprite.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundWaveMaterial(TEXT("/Game/Assets/Player/Wave"));
	if (FoundWaveMaterial.Succeeded())
		WaveMaterial = FoundWaveMaterial.Object;
}

void AWave::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	DynamicMaterial = UMaterialInstanceDynamic::Create(WaveMaterial, this);
	DynamicMaterial->GetScalarParameterValue("InnerRadius", InnerRadius);
	Sprite->SetMaterial(0, DynamicMaterial);
}

// Called when the game starts or when spawned
void AWave::BeginPlay()
{
	Super::BeginPlay();

	// We use the AddDynamic macro to create a delegate for collision
	OnActorBeginOverlap.AddDynamic(this, &AWave::BeginOverlap);
}

// Called every frame
void AWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We overwrite the movement from the parent
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
	// We scale the actor over time
	SetActorScale3D(GetActorScale3D() + DeltaTime);

	// We make it thinner over time to maintain a nice wave curvature instead of a chunky one
	InnerRadius += DeltaTime * 0.025;
	DynamicMaterial->SetScalarParameterValue("InnerRadius", InnerRadius);
}

void AWave::BeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	// If it hits an enemy, we deal damage to it and destroy the projectile
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
		Gloom->Damage(Damage);

	// If we hit a tower that it is not in building mode we call the Photon(FRotator Rotation)
	if (CanOverlap && Parent != OtherActor)
	{
		ATower* Tower = Cast<ATower>(OtherActor);
		if (Tower)
		{
			if (!Tower->IsInBuildinMode)
			{
				if (Tower->CanShoot)
				{
					Tower->CanShoot = false;
					Tower->Sprite->SetSpriteColor(Sprite->GetSpriteColor());
					Tower->Damage = Damage;
					Tower->Wave(GetActorRotation());
				}
				Destroy();
			}
		}
		// If we hit the player objective, we simply destroy the projectile
		else if (Cast<APlayerObjective>(OtherActor))
			Destroy();
	}
}

