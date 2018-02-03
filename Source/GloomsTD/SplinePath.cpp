// Fill out your copyright notice in the Description page of Project Settings.

#include "SplinePath.h"
#include "Gloom.h"
#include "GloomsTDGameModeBase.h"

// Sets default values
ASplinePath::ASplinePath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the component that handles the spline
	this->Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	// We search for a specific file in the game folder
	// If the file is found we set it to a certain object and/or save it for later in a variable
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundStaticMesh(TEXT("/Game/Assets/Spline/road"));
	if (FoundStaticMesh.Succeeded())
		StaticMesh = FoundStaticMesh.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Assets/Spline/RoadMaterial"));
	if (FoundMaterial.Succeeded())
		Material = FoundMaterial.Object;
}

void ASplinePath::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	/*
	 * For each point we create a start and and where we spawn a SplineMesh Component. 
	 * A spline mesh component streatches a mesh to fit the spline.
	 * In this way, I create the road from the game and I can make it take any shape and keep a nice look.
	 */
	for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent* Mesh = NewObject<USplineMeshComponent>(Spline);
		Mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		Mesh->SetMobility(EComponentMobility::Movable);
		Mesh->SetStaticMesh(StaticMesh);
		Mesh->SetMaterial(0, Material);
		Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		Mesh->SetForwardAxis(ESplineMeshAxis::X, true);
		Mesh->SetStartAndEnd(Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local), Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local), Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local), Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local), true);
		Mesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
		Mesh->RegisterComponent();
	}
}

// Called when the game starts or when spawned
void ASplinePath::BeginPlay()
{
	Super::BeginPlay();
	
	// Construction scripts runs only when spawned in the world not preplaced in the world so I tell it to run
	RerunConstructionScripts();

	// We add a reference of this spline
	Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->Spline.Add(this);
}

// Called every frame
void ASplinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We check if all the enemies are killed and end wave
	if (EnemiesKilled >= EnemiesToKill && EnemiesToKill != 0)
		EndWave();
}

void ASplinePath::StartWave()
{
	// We generate the number of enemies for the next wave
	EnemiesToSpawn = Sequence(Wave);
	EnemiesToKill = EnemiesToSpawn;

	// We se a timer to spawn enemies each second
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ASplinePath::SpawningTimer, 1.0f, true);
}

void ASplinePath::SpawningTimer()
{
	// If there still are enemies to spawn, we spawn them else we clear the timer
	if (EnemiesToSpawn > 0)
	{
		// We know that we spawned one more enemy
		EnemiesToSpawn--;
		// We spawn the enemy with a random RGBA value for color based on the reached wave
		SpawnGloom(FColor(FMath::RandRange(((5 * (Wave - 1) < 256) ? 0 : (5 * (Wave - 1) - 255)), ((5 * (Wave - 1) < 256) ? (5 * (Wave - 1)) : 255)),
			FMath::RandRange(((5 * (Wave - 1) < 256) ? 0 : (5 * (Wave - 1) - 255)), ((5 * (Wave - 1) < 256) ? (5 * (Wave - 1)) : 255)),
			FMath::RandRange(((5 * (Wave - 1) < 256) ? 0 : (5 * (Wave - 1) - 255)), ((5 * (Wave - 1) < 256) ? (5 * (Wave - 1)) : 255)),
			FMath::RandRange(((5 * (Wave - 1) < 256) ? 1 : (5 * (Wave - 1) - 255)), ((5 * (Wave - 1) < 256) ? (5 * (Wave - 1)) : 255))));
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);
}

void ASplinePath::SpawnGloom(FColor Color)
{
	// We spawn the enemy and intialize it
	FVector Location = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AGloom* SpawnedGloom = GetWorld()->SpawnActor<AGloom>(Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	if (SpawnedGloom)
		SpawnedGloom->PostSpawn(this, Color);
}

void ASplinePath::EndWave()
{
	// We reset the kill count and tell that this wave is done
	EnemiesKilled = 0;
	Cast<AGloomsTDGameModeBase>(GetWorld()->GetAuthGameMode())->EndWave();
}

FString ASplinePath::BitsToString(int n)
{
	n = FMath::Pow(n, FMath::FloorToInt(n / 2));
	FString Bits;
	//Reserveing space for string in memory FString won't rellocate string multiple times in process
	Bits.Reserve(FMath::FloorToInt(FMath::Sqrt(n)));
	//We multiply by 2 until it's bigger then n where every bit will be 0 after that
	for (int i = 1; i<n; i = i * 2)
	{
		if (n & i)
			Bits.Append("1");
		else
			Bits.Append("0");
	}
	//We need to reverse bit order, so smallest is last
	return Bits.Reverse();
}

int ASplinePath::Count(FString String, FString ElementToCount)
{
	int count = 0;
	for (int i = 0; i < String.Len(); i++)
		if (String[i] == ElementToCount[0])
			count++;
	return count;
}

bool ASplinePath::IsPrime(int n)
{
	bool isPrime = true;
	for (int i = 2; i <= n / 2; i++)
		if (n % i == 0)
		{
			isPrime = false;
			break;
		}
	return isPrime;
}

int ASplinePath::Sequence(int n)
{
	int i = 1;
	int j = 1;
	while (j <= n)
	{
		if (IsPrime(Count(BitsToString(i), "1")))
			j++;
		i++;
	}
	return i + 18;
}