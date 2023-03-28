// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawnPoint.h"
#include "Pickup.h"

// Sets default values
APickupSpawnPoint::APickupSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	if(bDelaySpawn)
	{
		// Start Timer on Delay spawn
		BeginPlayDelaySpawnStart();
	}
	else
	{
		SpawnPickup();
	}
	
}

// Spawn out pickup in World
void APickupSpawnPoint::SpawnPickup()
{
	int32 NumPickupClasses = PickupClasses.Num();
	if(NumPickupClasses > 0 )
	{
		int32 Selection = 0;
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[0], GetActorTransform());
		if(SpawnedPickup)
		{
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnPoint::SpawnPickupTimerStart);
		}
	}
}

// Finished respawn timer
void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	SpawnPickup();
}

// Start respawn timer 
void APickupSpawnPoint::SpawnPickupTimerStart(AActor* DestroyedActor)
{
	GetWorldTimerManager().SetTimer(
		SpawnPickupTimer,
		this,
		&APickupSpawnPoint::SpawnPickupTimerFinished,
		RespawnPickupTime);
}

// Start delay started spawn 
void APickupSpawnPoint::BeginPlayDelaySpawnStart()
{
	GetWorldTimerManager().SetTimer(
		BeginPlayDelaySpawn,
		this,
		&APickupSpawnPoint::BeginPlayDelaySpawnFinished,
		DelaySpawnPickup);
}

// Finish delay started spawn 
void APickupSpawnPoint::BeginPlayDelaySpawnFinished()
{
	// When delay spawn finished, call SpawnPickup, which on Event destroy set respawn timer
	SpawnPickup();
}

// Called every frame
void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

