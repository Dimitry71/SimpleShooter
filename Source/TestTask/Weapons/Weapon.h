// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "TestTask/Weapons/WeaponTypes.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};
UCLASS()
class TESTTASK_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Fire(const FVector& HitTarget);
	virtual void PlayFireSound();
	

	/*
	* Setting crosshair for Weapon
	*/
	UPROPERTY(EditAnywhere,Category= "Crosshairs")
	class UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere,Category= "Crosshairs")
	class UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere,Category= "Crosshairs")
	class UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere,Category= "Crosshairs")
	class UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere,Category= "Crosshairs")
	class UTexture2D* CrosshairBottom;

	/** 
	* Automatic fire
	*/
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bAutomatic = true;

	UPROPERTY()
	class AMainCharacter* CharacterOwner;
	UPROPERTY()
	class AMainCharacterController* CharacterController;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(VisibleAnywhere)
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category= "Weapon Sounds")
	USoundCue* WeaponNormalFireSound;

	UPROPERTY(EditAnywhere, Category= "Weapon Sounds")
	USoundCue* WeaponLowAmmoFireSound;

	UPROPERTY(EditAnywhere, Category= "Weapon Sounds")
	USoundCue* WeaponLastAmmoFireSound;

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	int32 LowAmmoIndicator;												

public:
	FORCEINLINE void SetWeaponSate(EWeaponState NewState) {WeaponState = NewState;}
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh;}
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType;}
};
