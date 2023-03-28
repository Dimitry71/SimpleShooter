// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "TestTask/Weapons/Weapon.h"
#include "TestTask/Components/TurnInPlace.h"
#include "TestTask/CharacterController/MainCharacterController.h"
#include "TestTask/HUD/TestTaskHUD.h"
#include "TestTask/Weapons/WeaponTypes.h"
#include "MainCharacter.generated.h"
#define TRACE_LENGTH 80000.f

UCLASS()
class TESTTASK_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	void PlayFireMontage();
	void PlayElimMontage();
	virtual void Elim();
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);
	virtual void PostInitializeComponents() override;
	void UpdateHUDHealth();
	bool CanFire();
	bool IsEmpty();
	bool IsFull();
	
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void Fire();
	
	// Set up inputs
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn (float Value);
	void LookUp(float Value);
	void CrouchButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();
	virtual void Jump() override;
	//Equip weaopon
	void EquipWeapon();
	// Showing in HUD
	void SetHUDCrosshairs(float DeltaTime);
	void SetHUDAmmo();
	virtual void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SpawnDefaultWeapon();
	void UpdateHUDAmmo();

	UPROPERTY(VisibleAnywhere, Category= "Camera")
	class AHitScanWeapon* EquippedWeapon;


	// Calculate AO_Yaw for Aim Offset
	void AimOffset(float DeltaSeconds);

	/*
	 * Automatic Fire
	 */
	FTimerHandle FireTimer;
	bool bCanFire = true;
	void StartFireTimer();
	void FireTimerFinished();
	void SpendBullet();
	virtual void ElimTimerFinished();
	
	
	

	TMap<EWeaponType,int32> CarriedAmmoMap;
	void InitializeCarriedAmmo();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FVector HitTarget;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* Health;

	UPROPERTY()
	AMainCharacterController* PlayerController;
	
	
private:
	
	
	UPROPERTY(EditAnywhere, Category= "Camera")
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category= "Camera")
	UCameraComponent* FollowCamera;

	

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHitScanWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 MaxCarriedAmmo;

	UPROPERTY(EditAnywhere)
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UAnimMontage* FiringWeapon;

	UPROPERTY(EditAnywhere, Category = "Elim")
	class UAnimMontage* ElimMontage;
	
	UPROPERTY()
	ATestTaskHUD* HUD;

	bool bFireButtonPressed;
	/*
	 *	Variables for turning in place
	 */
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 1.5f;

	ETurningInPlace TurnInPlace;
	void TurningInPlace(float DeltaTime);
public:
	FORCEINLINE const float GetAO_Yaw() { return AO_Yaw;}
	FORCEINLINE const float GetAO_Pitch() { return AO_Pitch;}
	FORCEINLINE const int32 GetCurrentAmmo() { return CurrentAmmo;}
	FORCEINLINE const int32 GetMaxCarriedAmmo() { return MaxCarriedAmmo;}
	FORCEINLINE void SetCurrentAmmo(int32 NewAmmo) { CurrentAmmo=NewAmmo;}
	FORCEINLINE const ETurningInPlace GetETurningInPlace() { return TurnInPlace;}
	AWeapon* GetEquippedWeapon(); 
};




