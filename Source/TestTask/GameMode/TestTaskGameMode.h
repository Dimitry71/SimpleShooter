// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "TestTaskGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API ATestTaskGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(class AMainCharacter* ElimmedCharacter, class AMainCharacterController* VictimController,
		AMainCharacterController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter , AController* ElimmedController);

	// Selecting random Player Start
	AActor* SelectRandPlayerStart();
	// Checking if in player start radius another player located
	bool CheckForOpponents(FVector SpawnLocation);
	UPROPERTY(EditAnywhere)
	float SpawnRadius;
	
};
