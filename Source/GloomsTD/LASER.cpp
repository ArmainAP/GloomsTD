// Fill out your copyright notice in the Description page of Project Settings.

#include "LASER.h"
#include "Gloom.h"
#include "Tower.h"
#include "PlayerObjective.h"

// Sets default values
ALASER::ALASER()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is a geometric that serves no purpose other than avoiding to make BoxComponent the root
	this->SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	this->RootComponent = SceneComponent;

	// This is the component that will handle the collision
	this->BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(1.0f, 5.0f, 5.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// This is the component that will handle the particle system
	this->Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(RootComponent);

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundParticle(TEXT("/Game/Assets/Player/LaserParticle"));
	if (FoundParticle.Succeeded())
		Particle->SetTemplate(FoundParticle.Object);
}

// Called when the game starts or when spawned
void ALASER::BeginPlay()
{
	Super::BeginPlay();

	// We use the AddDynamic macro to create a delegate for collision
	OnActorBeginOverlap.AddDynamic(this, &ALASER::BeginOverlap);
}

// Called every frame
void ALASER::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set the actor rotation
	if (Parent && Parent->IsA(APlayerObjective::StaticClass()))
		SetActorRotation(Parent->GetActorRotation() + Offset);
	// Updates the beam 
	SetBeam();
}

void ALASER::SetBeam()
{
	// Set the default beam lenght around the viewport time
	EndPoint = GetActorForwardVector() * 1280.0f + GetActorLocation();
	EndPoint.X = FMath::Clamp(EndPoint.X, -640.0f, 640.0f);
	EndPoint.Y = FMath::Clamp(EndPoint.Y, -360.0f, 360.0f);
	float SizeClamped = (EndPoint - GetActorLocation()).Size();

	// Trace to see if the beam is hitting anything
	FHitResult Hit;
	FCollisionQueryParams params = FCollisionQueryParams(FName(), true);
	params.bTraceComplex = true;
	params.AddIgnoredActor(Parent);
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, GetActorLocation(), GetActorForwardVector() * SizeClamped + GetActorLocation(), TraceObjectTypes, params);
	if (bHit)
	{
		ATower* Tower = Cast<ATower>(Hit.GetActor());
		if(Tower)
			if (!Tower->IsInBuildinMode)
			{
				if (Tower->CanShoot)
				{
					FLinearColor Color;
					Particle->GetColorParameter("Color", Color);
					Tower->Sprite->SetSpriteColor(Color);
					Tower->Damage = Damage;
					Tower->Laser(GetActorRotation());
				}
				EndPoint = Hit.GetActor()->GetActorLocation();
				SizeClamped = (EndPoint - GetActorLocation()).Size();
			}
	}
	
	// Set the sizes for collision box and beam lenght
	BoxComponent->SetRelativeScale3D(FVector(SizeClamped / 2, 1.0f, 1.0f));
	BoxComponent->SetRelativeLocation(FVector(SizeClamped / 2, 0.0f, 0.0f));
	Particle->SetBeamEndPoint(0, GetActorForwardVector() * SizeClamped + GetActorLocation());
}

void ALASER::BeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	// Deals damage to the enemy
	AGloom* Gloom = Cast<AGloom>(OtherActor);
	if (Gloom)
		Gloom->Damage(Damage);
}