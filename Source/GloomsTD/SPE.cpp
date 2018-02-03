// Fill out your copyright notice in the Description page of Project Settings.

#include "SPE.h"
#include "SPEPhoton.h"
#include "Gloom.h"

ASPE::ASPE()
{
	// We set the base buy price for the tower
	BasePrice = 20;

	// We set the collision profile and sphere radius
	SphereComponent->SetSphereRadius(160.0f);
	SphereComponent->SetCollisionProfileName("OverlapOnlyPawn");

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Towers/SPE_Sprite"));
	if (FoundSprite.Succeeded())
	{
		Sprite->SetSprite(FoundSprite.Object);
		Glow->SetSprite(FoundSprite.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Assets/Gloom/Shield"));
	if (FoundMaterial.Succeeded())
		GlowMaterial = FoundMaterial.Object;
}

void ASPE::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	DynamicMaterial = UMaterialInstanceDynamic::Create(GlowMaterial, this);
	DynamicMaterial->SetScalarParameterValue("Radius", 0.2f);
	Glow->SetMaterial(0, DynamicMaterial);

	// We update the game sprite for Tier 1
	UpdateSprite(1);
}

void ASPE::UpdateSprite(int Tier)
{
	// We set the glow color based on upgrade tier.
	switch (Tier)
	{
	case 1:
		Glow->SetSpriteColor(FColor(255, 0, 0, 255));
		break;

	case 2:
		Glow->SetSpriteColor(FColor(255, 255, 0, 255));
		break;

	case 3:
		Glow->SetSpriteColor(FColor(255, 255, 255, 255));
		break;
	}
}

void ASPE::BeginPlay()
{
	Super::BeginPlay();

	// We use the AddDynamic macro to create a delegate for collision
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASPE::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASPE::EndOverlap);
}

void ASPE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We adjust rotation towards the target if we can shoot and we have a valid target
	if (Glooms.Num() > 0 && !CanShoot)
		SetActorRotation((Glooms[0]->GetActorLocation() - GetActorLocation()).Rotation() + FRotator(0.0f, 90.0f, 0.0f));
}

void ASPE::Photon(FRotator Rotation)
{
	Super::Photon(Rotation);

	// We call the event that handles the projectile
	Shoot();
}

void ASPE::Wave(FRotator Rotation)
{
	Super::Wave(Rotation);

	// We call the event that handles the projectile
	Shoot();
}

void ASPE::Laser(FRotator Rotation)
{
	Super::Laser(Rotation);

	// We call the event that handles the projectile, and we no longer allow it to shoot additional ones to avoid duplication.
	CanShoot = false;
	Shoot();
}

void ASPE::Shoot()
{
	// We spawn the projectile and initialize it
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASPEPhoton* SPEPhoton = GetWorld()->SpawnActor<ASPEPhoton>(Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	if (SPEPhoton)
	{
		if(Glooms.Num() > 0)
			SPEPhoton->Target = Glooms[0];
		SPEPhoton->Damage = 4 * Tier;
		SPEPhoton->SetActorRotation(GetActorRotation() - FRotator(0.0f, 90.0f, 0.0f));
	}
}

void ASPE::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If we begin overlap with an enemy, we add a unique reference of it in an array
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
		Glooms.AddUnique(Gloom);
}

void ASPE::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// If we end overlap with an enemy, we remove the reference from the array
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
		Glooms.Remove(Gloom);
}
