// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDAmmo(int32 Ammo);
	void SetHUDHealth(float Health, float MaxHealth);
	virtual void OnPossess(APawn* InPawn) override;
	void PollInit();

	
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
private:
	
	UPROPERTY()
	class ATestTaskHUD* TestTaskHUD;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	
	float HUDHealth;
	float HUDMaxHealth;
	float HUDCurrentAmmo;
	bool bInitializeCurrentAmmo = false;
	bool bInitializeCharacterOverlay = false;
};
