// Fill out your copyright notice in the Description page of Project Settings.


#include "BotsSpawner.h"
#include "TestTask/ArtificalIntelegence/EnemyAICharacter.h"
#include "TestTask/Components/HealthComponent.h"
#include "TestTask/GameMode/TestTaskGameMode.h"

// Sets default values
ABotsSpawner::ABotsSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABotsSpawner::BeginPlay()
{
	Super::BeginPlay();
	ATestTaskGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestTaskGameMode>();
	if(GameMode)
	{
		CurrentPlayerStart = GameMode->SelectRandPlayerStart();
		while(!GameMode->CheckForOpponents(CurrentPlayerStart->GetActorLocation()))
		{
			CurrentPlayerStart = GameMode->SelectRandPlayerStart();
		}
	}
	FActorSpawnParameters SpawnParams;
	const FVector SpawnLocation = CurrentPlayerStart->GetActorLocation();
	const FRotator SpawnRotation = CurrentPlayerStart->GetActorRotation();
	EnemyAICharacter = GetWorld()->SpawnActor<AEnemyAICharacter>(EnemyAICharacterClass,SpawnLocation,SpawnRotation,SpawnParams);
	
}

// Called every frame
void ABotsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(EnemyAICharacter->Health->IsElimmed())
	{
		ATestTaskGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestTaskGameMode>();
		if(GameMode)
		{
			CurrentPlayerStart = GameMode->SelectRandPlayerStart();
			while(!GameMode->CheckForOpponents(CurrentPlayerStart->GetActorLocation()))
			{
				CurrentPlayerStart = GameMode->SelectRandPlayerStart();
			}
		}
		FActorSpawnParameters SpawnParams;
		const FVector SpawnLocation = CurrentPlayerStart->GetActorLocation();
		const FRotator SpawnRotation = CurrentPlayerStart->GetActorRotation();
		EnemyAICharacter = GetWorld()->SpawnActor<AEnemyAICharacter>(EnemyAICharacterClass,SpawnLocation,SpawnRotation,SpawnParams);
	}

}

void ABotsSpawner::TimerElimFinished()
{
	ATestTaskGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestTaskGameMode>();
	if(GameMode)
	{
		CurrentPlayerStart = GameMode->SelectRandPlayerStart();
		while(!GameMode->CheckForOpponents(CurrentPlayerStart->GetActorLocation()))
		{
			CurrentPlayerStart = GameMode->SelectRandPlayerStart();
		}
	}
	FActorSpawnParameters SpawnParams;
	const FVector SpawnLocation = CurrentPlayerStart->GetActorLocation();
	const FRotator SpawnRotation = CurrentPlayerStart->GetActorRotation();
	EnemyAICharacter = GetWorld()->SpawnActor<AEnemyAICharacter>(EnemyAICharacterClass,SpawnLocation,SpawnRotation,SpawnParams);
}

