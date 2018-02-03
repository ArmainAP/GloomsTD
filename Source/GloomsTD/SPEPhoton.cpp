// Fill out your copyright notice in the Description page of Project Settings.

#include "SPEPhoton.h"
#include "Gloom.h"

// Sets default values
ASPEPhoton::ASPEPhoton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the component that will handle the collision
	this->SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetSphereRadius(9.0f);
	this->RootComponent = SphereComponent;

	// This is the component that renders the sprite
	this->Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));

	// We set collision profiles
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

void ASPEPhoton::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We create and set a dynamic material texture that we can modify during runtime
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PhotonMaterial, this);
	DynamicMaterial->SetScalarParameterValue("Radius", 0.5);
	Sprite->SetMaterial(0, DynamicMaterial);

	// We set the sprite color to black
	Sprite->SetSpriteColor(FColor(0, 0, 0, 255));
}

void ASPEPhoton::BeginPlay()
{
	Super::BeginPlay();

	// We use the AddDynamic macro to create a delegate for collision
	OnActorBeginOverlap.AddDynamic(this, &ASPEPhoton::BeginOverlap);
}

void ASPEPhoton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If target is valid and not pending to be destroyed, we get the location of else we simply destroy the projectile
	if (Target && Target->IsValidLowLevel() && !Target->IsPendingKill())
		TargetLocation = Target->GetActorLocation();
	else
		Destroy();

	// We rotate the projectile towards the target
	SetActorRotation((TargetLocation - GetActorLocation()).Rotation());
	// We move the projectile forward
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 500 * DeltaTime);
}

void ASPEPhoton::BeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	// If we hit an enemy, we deal damage and destroy the projectile
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
	{
		Gloom->Damage(Damage);
		Destroy();
	}
}