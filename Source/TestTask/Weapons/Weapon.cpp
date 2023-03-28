// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Kismet/GameplayStatics.h"
#include "TestTask/Character/MainCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AWeapon::Fire(const FVector& HitTarget)
{
	bool bIsFireSoundsValid = WeaponNormalFireSound && WeaponLowAmmoFireSound && WeaponLastAmmoFireSound; 
	if(FireAnimation && bIsFireSoundsValid)
	{
		WeaponMesh->PlayAnimation(FireAnimation,false);
		PlayFireSound();
	}
}

void AWeapon::PlayFireSound()
{
	CharacterOwner = CharacterOwner == nullptr ? Cast<AMainCharacter>(GetOwner()) : CharacterOwner;
	if(CharacterOwner)
	{
		if(CharacterOwner->GetCurrentAmmo() > LowAmmoIndicator)
		{
			// Play normal fire sound
			UGameplayStatics::PlaySoundAtLocation(
				this,
				WeaponNormalFireSound,
				GetActorLocation());
		}
		else if(CharacterOwner->GetCurrentAmmo() <= LowAmmoIndicator)
		{
			// Play low ammo fire sound
			UGameplayStatics::PlaySoundAtLocation(
				this,
				WeaponLowAmmoFireSound,
				GetActorLocation());
			if (CharacterOwner->GetCurrentAmmo() == 1)
			{	
				// Play last ammo fire sound
				UGameplayStatics::PlaySoundAtLocation(
					this,
					WeaponLastAmmoFireSound,
					GetActorLocation());
		}
		}
		
	}
}

