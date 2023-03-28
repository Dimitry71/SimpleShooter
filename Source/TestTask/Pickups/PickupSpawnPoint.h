// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class TESTTASK_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupSpawnPoint();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickup>> PickupClasses;

	UPROPERTY()
	APickup* SpawnedPickup;

	void SpawnPickup();
	void SpawnPickupTimerFinished();
	UFUNCTION()
	void SpawnPickupTimerStart(AActor* DestroyedActor);

	void BeginPlayDelaySpawnStart();
	void BeginPlayDelaySpawnFinished();
private:
	FTimerHandle SpawnPickupTimer;

	FTimerHandle BeginPlayDelaySpawn;
	
	// Respawn time
	UPROPERTY(EditAnywhere)
	float RespawnPickupTime;

	// Begin play spawn time
	UPROPERTY(EditAnywhere)
	float DelaySpawnPickup;

	UPROPERTY(EditAnywhere)
	bool bDelaySpawn;
public:
	
	


};
