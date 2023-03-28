// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TestTask/Character/MainCharacter.h"
#include "TestTask/Components/HealthComponent.h"
#include "TestTask/CharacterController/MainCharacterController.h"

void ATestTaskGameMode::PlayerEliminated(AMainCharacter* ElimmedCharacter, AMainCharacterController* VictimController,
	AMainCharacterController* AttackerController)
{
	if(ElimmedCharacter)
	{
		ElimmedCharacter->Health->Elim();
	}
}

void ATestTaskGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if(ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if(ElimmedController)
	{
		AActor* CurrentPlayerStart = SelectRandPlayerStart();
		// Find first correct Player Start
		while(!CheckForOpponents(CurrentPlayerStart->GetActorLocation()))
		{
			// Set this correct Player Start
			CurrentPlayerStart = SelectRandPlayerStart();
		}
		RestartPlayerAtPlayerStart(ElimmedController,CurrentPlayerStart);
	}
	
}

AActor* ATestTaskGameMode::SelectRandPlayerStart()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this,APlayerStart::StaticClass(),PlayerStarts);
	int32 Selection = FMath::RandRange(0,PlayerStarts.Num()-1);
	return PlayerStarts[Selection];
}

bool ATestTaskGameMode::CheckForOpponents(FVector SpawnLocation)
{
	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainCharacter::StaticClass(), OverlappingActors);
		for (AActor* Actor : OverlappingActors)
		{
			if ((Actor->GetActorLocation() - SpawnLocation).Size() <= SpawnRadius)
			{
				return false;
			}
		}
		return true;
}
