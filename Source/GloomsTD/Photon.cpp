// Fill out your copyright notice in the Description page of Project Settings.

#include "Photon.h"
#include "PlayerObjective.h"
#include "Gloom.h"
#include "Tower.h"

APhoton::APhoton()
{
	// We set the speed multiplier
	Speed = 500;

	// We set the collision profiles
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Sprite->SetCollisionProfileName(TEXT("NoCollision"));

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Gloom/Gloom_Spr"));
	if (FoundSprite.Succeeded())
		Sprite->SetSprite(FoundSprite.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundPhotonMaterial(TEXT("/Game/Assets/Gloom/Shield"));
	if (FoundPhotonMaterial.Succeeded())
		PhotonMaterial = FoundPhotonMaterial.Object;
}

void APhoton::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PhotonMaterial, this);
	DynamicMaterial->SetScalarParameterValue("Radius", 0.5);
	Sprite->SetMaterial(0, DynamicMaterial);
}

void APhoton::BeginPlay()
{
	Super::BeginPlay();

	// We use the AddDynamic macro to create a delegate for collision
	OnActorBeginOverlap.AddDynamic(this, &APhoton::BeginOverlap);
}

void APhoton::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// If it hits an enemy, we deal damage to it and destroy the projectile
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
	{
		Gloom->Damage(Damage);
		Destroy();
	}

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
					Tower->Photon(GetActorRotation());
				}
				Destroy();
			}
		}
		// If we hit the player objective, we simply destroy the projectile
		else if (Cast<APlayerObjective>(OtherActor))
			Destroy();
	}
}