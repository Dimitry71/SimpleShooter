// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BotsSpawner.generated.h"

UCLASS()
class TESTTASK_API ABotsSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABotsSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FTimerHandle TimerElim;
	void TimerElimFinished();
	UPROPERTY(EditAnywhere)
	float ElimDelay = 1.5f;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyAICharacter> EnemyAICharacterClass;

	UPROPERTY()
	AActor* CurrentPlayerStart;

	UPROPERTY()
	AEnemyAICharacter* EnemyAICharacter;
};
