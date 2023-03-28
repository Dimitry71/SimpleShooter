// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAICharacter.h"

#include "EnemyCharacterAIController.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TestTask/GameMode/TestTaskGameMode.h"
#include "TestTask/Weapons/HitScanWeapon.h"


void AEnemyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	AIPlayerController = Cast<AEnemyCharacterAIController>(Controller);
	
}


void AEnemyAICharacter::Fire()
{
	Super::Fire();
	FVector TargetDirection = FVector(AIPlayerController->PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	FVector StrafeDirection = FVector(TargetDirection.Y, -TargetDirection.X, 0.0f);
	AddMovementInput(StrafeDirection, StrafeSpeed);
}

void AEnemyAICharacter::ElimTimerFinished()
{
	ATestTaskGameMode* TestTaskGameMode = GetWorld()->GetAuthGameMode<ATestTaskGameMode>();
}

void AEnemyAICharacter::Elim()
{
	GetWorldTimerManager().SetTimer(
		TimerElim,
		this,
		&AEnemyAICharacter::ElimTimerFinished,
		DelayElim);
	PlayElimMontage();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->Destroy();
	Destroy();
}
