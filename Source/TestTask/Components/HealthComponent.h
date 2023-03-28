// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestTask/CharacterController/MainCharacterController.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTTASK_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class AMainCharacter;
	// Call when Health == 0
	void Elim();
	virtual void Destroyed();
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor,float Damage,const UDamageType* DamageType,class AController* InstigatorController,AActor* DamageCauser);
	auto UpdateHUDHealth() -> void;

	UPROPERTY()
	AMainCharacterController* CharacterController;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AMainCharacter* Character;
	UPROPERTY()
    class AMainCharacterController* PlayerController;
    UPROPERTY()
    class ATestTaskHUD* HUD;

	UPROPERTY(EditAnywhere, Category= "PLayer Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere,Category= "PLayer Stats")
	float Health = 100.f;
	bool bElimmed = false;

	
public:
	FORCEINLINE const float GetHealth() { return Health;}
	FORCEINLINE const float GetMaxHealth() { return MaxHealth;}
	UFUNCTION(BlueprintCallable)
	const bool IsElimmed() {return bElimmed;}
	
	

		
};
