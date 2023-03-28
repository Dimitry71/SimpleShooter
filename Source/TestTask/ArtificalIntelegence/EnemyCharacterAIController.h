// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAICharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "TestTask/Character/MainCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyCharacterAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API AEnemyCharacterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyCharacterAIController();
	
	virtual void Possess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
	
	UPROPERTY()
	FVector PlayerLocation;
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UPROPERTY()
	AMainCharacter* PlayerCharacter;

	UPROPERTY()
	class AEnemyAICharacter* EnemyAICharacter;
	
protected:

	UPROPERTY()
	AActor* CurrentPlayerStart;
	
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UBehaviorTreeComponent* AIBehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UBlackboardComponent* AIBlackboardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UAIPerceptionComponent* AIPerceptionComponent;
	


private:
	void ChooseNavigationPoint();
	void ChooseNearAmmoPickup();


	
	
};
