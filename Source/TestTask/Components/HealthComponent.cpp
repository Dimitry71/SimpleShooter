// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "TestTask/GameMode/TestTaskGameMode.h"
#include "TestTask/Character/MainCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if(Character)
	{
		Character->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::ReceiveDamage);
	}
	

	// ...
	
}



// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::Elim()
{
	Character->Elim();	
	bElimmed = true;							
	
}

void UHealthComponent::Destroyed()
{
	
}

void UHealthComponent::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage,0.f,MaxHealth);
	UpdateHUDHealth();
	if(Health == 0.f)
	{
		ATestTaskGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ATestTaskGameMode>();
		if(BlasterGameMode)
		{
			CharacterController = CharacterController == nullptr ? Cast<AMainCharacterController>(Character->Controller) : CharacterController							;
			AMainCharacterController* AttackerController = Cast<AMainCharacterController>(InstigatorController);
			BlasterGameMode->PlayerEliminated(Character,CharacterController,AttackerController);
		}
	}
}

void UHealthComponent::UpdateHUDHealth()
{
	CharacterController = CharacterController == nullptr? Cast<AMainCharacterController>(Character->Controller) : CharacterController;
	if(CharacterController)
	{
		CharacterController->SetHUDHealth(Health, MaxHealth);
	}
}

