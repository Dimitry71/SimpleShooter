// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TestTask/Components/TurnInPlace.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(BlueprintReadOnly,Category = Character,meta =(AllowPrivateAccess = "true"))
	class AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadOnly,Category = Movement,meta =(AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(BlueprintReadOnly,Category = Movement,meta =(AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly,Category = Movement,meta =(AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	float YawOffset;

	FRotator DeltaRotation;
	class AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	ETurningInPlace TurnInPlace;

	UPROPERTY(BlueprintReadOnly,Category=Movement,meta =(AllowPrivateAccess = "true"))
	bool bElimmed;
	
	
	
	
};
