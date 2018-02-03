// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
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
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We move the projectile
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);

	// We check if the projectile is still in the screen area, if not, we destroy it
	if ((GetActorLocation().X > 640) || (GetActorLocation().X < -640) || (GetActorLocation().Y > 360) || (GetActorLocation().Y < -360))
		Destroy();
}