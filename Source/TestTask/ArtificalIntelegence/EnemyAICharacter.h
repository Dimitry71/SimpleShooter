// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestTask/Character/MainCharacter.h"
#include "EnemyAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API AEnemyAICharacter : public AMainCharacter
{
	GENERATED_BODY()
public:
	
	
	virtual void Fire() override;
	virtual void Elim() override;
	virtual void ElimTimerFinished() override;
	
	
	FTimerHandle TimerElim;
	UPROPERTY(EditAnywhere)
	float DelayElim = 1.5f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY()
	class AEnemyCharacterAIController* AIPlayerController;
	
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	float StrafeSpeed = 100.f;
public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() {return BehaviorTree;}
	
	
	
	
};
