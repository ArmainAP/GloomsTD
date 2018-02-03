// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "GloomsTDGameModeBase.h"
#include "SplinePath.h"

// Sets default values
ATower::ATower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the component that will handle the collision
	this->SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SphereComponent->SetSphereRadius(80.0f);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	this->RootComponent = SphereComponent;

	// This is the component that renders the sprite
	this->Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
	Sprite->SetSpriteColor(FColor(0, 0, 0, 255));
	Sprite->SetCollisionProfileName(TEXT("OverlapAll"));

	// This is the component that renders the sprite
	this->Glow = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Sprite);
	Glow->SetWorldLocation(FVector(0.0f, 0.0f, -1.0f));
	Glow->SetWorldScale3D(FVector(4.0f, 4.0f, 4.0f));
	Glow->SetSpriteColor(FColor(0, 0, 0, 255));
	Glow->SetCollisionProfileName(TEXT("NoCollision"));
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsInBuildinMode)
	{
		// In building mode the tower moves along with the cursor location
		FVector WorldLocation;
		FVector WorldDirection;
		GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		SetActorLocation(FVector(WorldLocation.X, WorldLocation.Y, 3.0f));

		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);

		// If there are no overlapping actors, we know we can place it
		if (OverlappingActors.Num() == 0)
			CanPlace = true;
		else
		{
			//If there are overlapping actors, we check the actor class and determine if we can place the tower there
			CanPlace = true;
			for (int i = 0; i < OverlappingActors.Num(); i++)
				if (OverlappingActors[i]->IsA(ATower::StaticClass()) || OverlappingActors[i]->IsA(ASplinePath::StaticClass()))
				{
					CanPlace = false;
					break;
				}
		}
	}
}

void ATower::UpdateSprite(int Tier)
{
	// This function is empty because it children override it and each have its own code
}

void ATower::Photon(FRotator Rotation)
{
	// We set a timer to let this tower shoot again after some time
	FTimerHandle AllowShootHandle;
	GetWorld()->GetTimerManager().SetTimer(AllowShootHandle, this, &ATower::CanShootOn, 0.25, false);
}

void ATower::Wave(FRotator Rotation)
{
	// We set a timer to let this tower shoot again after some time
	FTimerHandle AllowShootHandle;
	GetWorld()->GetTimerManager().SetTimer(AllowShootHandle, this, &ATower::CanShootOn, 0.5f, false);
}

void ATower::Laser(FRotator Rotation)
{
	// We set a timer to let this tower shoot again after some time
	FTimerHandle AllowShootHandle;
	GetWorld()->GetTimerManager().SetTimer(AllowShootHandle, this, &ATower::CanShootOn, 0.4f, false);
}

void ATower::CanShootOn()
{
	// We know we can shoot again
	CanShoot = true;
}

void ATower::CalculateUpgradePrice()
{
	// We calculate the upgrade price based on the BasePrice multiplie to 1.5 to the power of number of towers multiplied by the tier and divided by 3
	UpgradePrice = BasePrice * FMath::Pow(1.5, Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->Towers * Tier / 3);
}