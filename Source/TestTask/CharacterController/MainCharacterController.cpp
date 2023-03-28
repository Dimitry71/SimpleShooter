// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "TestTask/HUD/TestTaskHUD.h"
#include "TestTask/Character/MainCharacter.h"
#include "TestTask/Components/HealthComponent.h"
#include "TestTask/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AMainCharacterController::SetHUDAmmo(int32 Ammo)
{
	TestTaskHUD = TestTaskHUD == nullptr ? Cast<ATestTaskHUD>(GetHUD()) : TestTaskHUD;
	bool bHUDValid = TestTaskHUD && TestTaskHUD->CharacterOverlay && TestTaskHUD->CharacterOverlay->AmmoAmount;
	if(bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		TestTaskHUD->CharacterOverlay->AmmoAmount->SetText(FText::FromString(AmmoText));
	}

	else
	{
		bInitializeCurrentAmmo = true;
		HUDCurrentAmmo = Ammo;
	}
}

void AMainCharacterController::SetHUDHealth(float Health, float MaxHealth)
{
	TestTaskHUD = TestTaskHUD == nullptr ? Cast<ATestTaskHUD>(GetHUD()) : TestTaskHUD;
	bool bHUDValid = TestTaskHUD && TestTaskHUD->CharacterOverlay
	&& TestTaskHUD->CharacterOverlay->HealthBar
	&& TestTaskHUD->CharacterOverlay->HealthText;
	if(TestTaskHUD) UE_LOG(LogTemp,Warning,TEXT("BlasterHUD valid"));
	if(TestTaskHUD && TestTaskHUD->CharacterOverlay)
	{
		UE_LOG(LogTemp,Warning,TEXT("TestTaskHUD->CharacterOverlay valid"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("TestTaskHUD->CharacterOverlay NOT valid"));
	}
	if(bHUDValid)
	{
		const float HealthPercent = Health/MaxHealth;
		TestTaskHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		TestTaskHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
		
	}
}

void AMainCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(InPawn);
	if(MainCharacter)
	{
		SetHUDHealth(MainCharacter->Health->GetHealth(),MainCharacter->Health->GetMaxHealth());
	}
}

void AMainCharacterController::PollInit()
{
	if(CharacterOverlay == nullptr)
	{
		if(TestTaskHUD && TestTaskHUD->CharacterOverlay)
		{
			CharacterOverlay = TestTaskHUD->CharacterOverlay;
			if(CharacterOverlay)
			{
				if (bInitializeCurrentAmmo) SetHUDAmmo(HUDCurrentAmmo);
			}
		}
	}
}

void AMainCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PollInit();
}

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();
	TestTaskHUD = Cast<ATestTaskHUD>(GetHUD());
}
