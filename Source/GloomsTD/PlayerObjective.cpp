// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerObjective.h"
#include "GloomsTDGameModeBase.h"
#include "Tower.h"
#include "SplinePath.h"
#include "Photon.h"
#include "Wave.h"
#include "LASER.h"

// Sets default values
APlayerObjective::APlayerObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the component that handles the collision
	this->SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SphereComponent->SetSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	this->RootComponent = SphereComponent;

	// This is the component that renders the sprite
	this->Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
	Sprite->SetCollisionProfileName(TEXT("OverlapAll"));

	// This is the component that renders the sprite
	this->Glow = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Sprite);
	Glow->SetWorldLocation(FVector(0.0f, 0.0f, -1.0f));
	Glow->SetCollisionProfileName(TEXT("NoCollision"));

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FoundSprite(TEXT("/Game/Assets/Player/PlayerObjectiveSprite"));
	if (FoundSprite.Succeeded())
	{
		Sprite->SetSprite(FoundSprite.Object);
		Glow->SetSprite(FoundSprite.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundGlowMaterial(TEXT("/Game/Assets/Gloom/Shield"));
	if (FoundGlowMaterial.Succeeded())
		GlowMaterial = FoundGlowMaterial.Object;
}

void APlayerObjective::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	// We set the color and the scale
	Sprite->SetSpriteColor(SpriteColor);
	Glow->SetSpriteColor(SpriteColor);
	Glow->SetWorldScale3D(FVector(4.0f, 4.0f, 4.0f));

	// We create and set a dynamic material texture that we can modify during runtime
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GlowMaterial, this);
	DynamicMaterial->SetScalarParameterValue("Radius", 0.2f);
	Glow->SetMaterial(0, DynamicMaterial);
}

// Called when the game starts or when spawned
void APlayerObjective::BeginPlay()
{
	Super::BeginPlay();

	// Construction scripts runs only when spawned in the world not preplaced in the world so I tell it to run
	RerunConstructionScripts();

	// We get the player controller to posses this (enables input)
	GetWorld()->GetFirstPlayerController()->Possess(this);
	// We handle the cursor visibility
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = ((PLATFORM_ANDROID || PLATFORM_IOS) ? false : true);

	// We set a timer to shoot projectiles
	GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &APlayerObjective::ShootingTimer, 0.3f, true);

	// We add a reference
	Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->PlayerObjective = this;
}

// Called every frame
void APlayerObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsClickPressed)
	{
		ClickDownTime += DeltaTime;
		// If the click was pressed and held down for more than 0.2 seconds, you will able to target where to shoot based on cursor location
		if (ClickDownTime > 0.2 && !BuildMode)
		{
			FVector WorldPosition;
			FVector WorldDirection;
			GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
			TouchLocation = FVector(WorldPosition.X, WorldPosition.Y, 2.0f);
			Rotation = (TouchLocation - FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f)).Rotation();
			SetActorRotation(Rotation);
		}
	}
}

void APlayerObjective::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Here I bind the left click pressed/released actions to functions using PlayerInputComponent
	InputComponent->BindAction("Click", IE_Pressed, this, &APlayerObjective::ClickPressed);
	InputComponent->BindAction("Click", IE_Released, this, &APlayerObjective::ClickReleased);
}

void APlayerObjective::ClickPressed()
{
	// We know that the click is pressed
	IsClickPressed = true;

	// We check what our cursor overlaps
	TArray<TEnumAsByte<EObjectTypeQuery>> oTypes;
	oTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	Cast<APlayerController>(GetWorld()->GetFirstPlayerController())->GetHitResultUnderCursorForObjects(oTypes, false, Hit);
}

void APlayerObjective::ClickReleased()
{
	// We are in build mode and we can place, then we place the tower
	if (BuildMode && BuildTower)
	{
		if (BuildTower->CanPlace)
		{
			BuildTower->IsInBuildinMode = false;
			BuildMode = false;
		}
	}
	// It just a fast click, we check what is under cursor and open the corresponding menu
	else if (ClickDownTime < 0.2)
	{
		ATower* Tower = Cast<ATower>(Hit.GetActor());
		if (Tower)
		{
			Tower->CalculateUpgradePrice();
			Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->ShowTowerMenu(Cast<ATower>(Hit.GetActor()));
		}
		else if (Cast<APlayerObjective>(Hit.GetActor()))
			Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->ShowPOMenu();
		else
			Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->ShowMainMenu();
	}
	
	// We know that the click is no longer pressed and reset the click down time
	IsClickPressed = false;
	ClickDownTime = 0.0f;
}

void APlayerObjective::ShootingTimer()
{
	switch (DirectionTier)
	{
	case 1:
		Shoot(Rotation); // We shoot towards the cursor
		break;

	case 2:
		for (int i = 0; i < 4; i++)
			Shoot(Rotation + FRotator(0.0f, 90.0f, 0.0f) * i); // We shoot towards the cursor and 3 addition direcetions
		break;

	case 3:
		for (int i = 0; i < 8; i++)
			Shoot(Rotation + FRotator(0.0f, 45.0f, 0.0f) * i); // We shoot towards the cursor and 3 addition direcetions
		break;
	}
}

void APlayerObjective::Shoot(FRotator Rotation)
{
	// We spawn the projectile
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 2.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// We shoot and initialize a photon, wave or laser based on tier
	switch (WeaponTier)
	{
	case 1:
	{
		APhoton* SpawnedPhoton = GetWorld()->SpawnActor<APhoton>(Location, Rotation, SpawnInfo);
		if (SpawnedPhoton)
		{
			SpawnedPhoton->Parent = this;
			SpawnedPhoton->CanOverlap = true;
			SpawnedPhoton->Damage = DamageTier;
			switch (DamageTier)
			{
			case 1:
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
				break;

			case 2:
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 255, 0, 255));
				break;

			case 3:
				SpawnedPhoton->Sprite->SetSpriteColor(FColor(255, 255, 255, 255));
				break;
			}
		}
		break;
	}

	case 2:
	{
		AWave* SpawnedWave = GetWorld()->SpawnActor<AWave>(Location, Rotation, SpawnInfo);
		if (SpawnedWave)
		{
			SpawnedWave->Parent = this;
			SpawnedWave->CanOverlap = true;
			SpawnedWave->Damage = DamageTier * 3;
			switch (DamageTier)
			{
			case 1:
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 0, 0, 255));
				break;

			case 2:
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 255, 0, 255));
				break;

			case 3:
				SpawnedWave->Sprite->SetSpriteColor(FColor(255, 255, 255, 255));
				break;
			}
		}
		break;
	}

	case 3:
	{
		ALASER* SpawnedLaser = GetWorld()->SpawnActor<ALASER>(Location, GetActorRotation() + Rotation, SpawnInfo);
		if (SpawnedLaser)
		{
			LaserChild.Add(SpawnedLaser);
			SpawnedLaser->Parent = this;
			SpawnedLaser->Damage = DamageTier * 5;
			SpawnedLaser->Offset = Rotation;
			switch (DamageTier)
			{
			case 1:
				SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 0, 0, 255));
				break;

			case 2:
				SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 255, 0, 255));
				break;

			case 3:
				SpawnedLaser->Particle->SetColorParameter("Color", FColor(255, 255, 255, 255));
				break;
			}
		}
	}
	}
}

void APlayerObjective::ShootLaser()
{
	// We clear the lasers
	if (LaserChild.Num() > 0)
	{
		for (int i = 0; i < LaserChild.Num(); i++)
			LaserChild[i]->Destroy();
		LaserChild.Empty();
	}

	switch (DirectionTier)
	{
	case 1:
		Shoot(FRotator(0.0f, 0.0f, 0.0f)); //We pass the laser's offset as rotation
		break;

	case 2:
		for (int i = 0; i < 4; i++)
			Shoot(FRotator(0.0f, 90.0f, 0.0f) * i); //We pass the laser's offset as rotation
		break;

	case 3:
		for (int i = 0; i < 8; i++)
			Shoot(FRotator(0.0f, 45.0f, 0.0f) * i); //We pass the laser's offset as rotation
		break;
	}
}