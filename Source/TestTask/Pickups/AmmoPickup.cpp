// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "TestTask/Character/MainCharacter.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweetResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweetResult);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if(MainCharacter)
	{
		MainCharacter->PickupAmmo(WeaponType, AmountAmmoToPickup);
	}
	Destroy();
}
