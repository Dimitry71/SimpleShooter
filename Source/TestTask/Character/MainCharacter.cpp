// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TestTask/CharacterController/MainCharacterController.h"
#include "TestTask/TestTask.h"
#include "TestTask/HUD/TestTaskHUD.h"
#include "TimerManager.h"
#include "TestTask/Character/MainCharacterAnimInstance.h"
#include "TestTask/Weapons/HitScanWeapon.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TestTask/GameMode/TestTaskGameMode.h"
#include "TestTask/Components/HealthComponent.h"
#include "TestTask/Weapons/Weapon.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Set Spring arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation =true;

	// Set camera and attach is to Spring Arm
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Set Control rotation for straifing
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Set correct collision
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);

	TurnInPlace = ETurningInPlace::ETIP_NotTurning;
	
}



// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultWeapon();
	PlayerController = Cast<AMainCharacterController>(Controller);
	if(PlayerController)
	{
		PlayerController->SetHUDHealth(Health->GetHealth(),Health->GetMaxHealth());
	}
	InitializeCarriedAmmo();
	SetHUDAmmo();
	Health->UpdateHUDHealth();
	
	
}

void AMainCharacter::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, CurrentAmmo);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
	HitTarget = HitResult.ImpactPoint;
}

// Start Timer for automatic fire
void AMainCharacter::StartFireTimer()
{
	if (EquippedWeapon == nullptr) return;
	GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&AMainCharacter::FireTimerFinished,
		EquippedWeapon->FireDelay
	);
}

// Finish Timer for automatic fire
void AMainCharacter::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;
	if (bFireButtonPressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}
}

void AMainCharacter::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		EquippedWeapon->Fire(HitTarget);
		SpendBullet();
		PlayFireMontage();
		StartFireTimer();
	}
}



bool AMainCharacter::CanFire()
{
	if(EquippedWeapon == nullptr) return false;
	return  !IsEmpty() && bCanFire;
}

bool AMainCharacter::IsEmpty()
{
	return CurrentAmmo <= 0;
}

bool AMainCharacter::IsFull()
{
	return CurrentAmmo == MaxCarriedAmmo;
}

void AMainCharacter::PlayFireMontage()
{
	if(EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FiringWeapon)
	{
		AnimInstance->Montage_Play(FiringWeapon);
	}
}

void AMainCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void AMainCharacter::SetHUDAmmo()
{
	PlayerController == nullptr ? Cast<AMainCharacterController>(Controller) : PlayerController;
	if(PlayerController)
	{
		PlayerController->SetHUDAmmo(CurrentAmmo);
	}
}

void AMainCharacter::SpendBullet()
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo -1 , 0, MaxCarriedAmmo);
	SetHUDAmmo();
}

void AMainCharacter::EquipWeapon()
{
	EquippedWeapon->SetWeaponSate(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if(RightHandSocket)
	{
		RightHandSocket->AttachActor(EquippedWeapon,GetMesh());
	}
	EquippedWeapon->SetOwner(this);
	SetHUDAmmo();
	if(CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		MaxCarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
}

AWeapon* AMainCharacter::GetEquippedWeapon()
{
	return EquippedWeapon;
}

// Setting crosshair for weapon
void AMainCharacter::SetHUDCrosshairs(float DeltaTime)
{
	if (PlayerController == nullptr) return;

	PlayerController = PlayerController == nullptr ? Cast<AMainCharacterController>(Controller) : PlayerController;
	if (PlayerController)
	{
		HUD = HUD == nullptr ? Cast<ATestTaskHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD)
		{
			FHUDPackage HUDPackage;
			
				HUDPackage.CrosshairCenter = EquippedWeapon->CrosshairCenter;
				HUDPackage.CrosshairLeft = EquippedWeapon->CrosshairLeft;
				HUDPackage.CrosshairRight = EquippedWeapon->CrosshairRight;
				HUDPackage.CrosshairBottom = EquippedWeapon->CrosshairBottom;
				HUDPackage.CrosshairTop = EquippedWeapon->CrosshairTop;
			
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void AMainCharacter::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X /2.0, ViewportSize.Y / 2.0);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this,0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection);
	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		float DistanceToCharacter = (GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12.f,
				FColor::Red
				);
		}
	}
}

void AMainCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if(World && WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AHitScanWeapon>(WeaponClass);
		EquipWeapon();
	}
}

void AMainCharacter::UpdateHUDAmmo()
{
	PlayerController = PlayerController == nullptr? Cast<AMainCharacterController>(Controller) : PlayerController;
	if(PlayerController)
	{
		PlayerController->SetHUDAmmo(CurrentAmmo);
	}
	
}




void AMainCharacter::Elim()
{
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&AMainCharacter::ElimTimerFinished,
		ElimDelay);
	PlayElimMontage();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	if(PlayerController)
	{
		DisableInput(PlayerController);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->Destroy();
}


void AMainCharacter::ElimTimerFinished()
{
	ATestTaskGameMode* TestTaskGameMode = GetWorld()->GetAuthGameMode<ATestTaskGameMode>();
	if(TestTaskGameMode)
	{
		TestTaskGameMode->RequestRespawn(this,Controller);
	}

}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Health)
	{
		Health->Character = this;
	}
}

void AMainCharacter::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	if(CarriedAmmoMap.Contains(WeaponType))
	{
		CurrentAmmo = FMath::Clamp(CarriedAmmoMap[WeaponType] + AmmoAmount, 0, MaxCarriedAmmo);
		UpdateHUDAmmo();
	}
	
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AMainCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AMainCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire",IE_Released,this,&AMainCharacter::FireButtonReleased);

	
	PlayerInputComponent->BindAxis("MoveForward",this,&AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&AMainCharacter::LookUp);

}

void AMainCharacter::MoveForward(float Value)
{
	if(Controller!= nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction,Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if(Controller!= nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction,Value);
	}
}

void AMainCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::CrouchButtonPressed()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AMainCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	if(EquippedWeapon)
	{
		Fire();
	}
}

void AMainCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}





void AMainCharacter::Jump()
{
	Super::Jump();
}



void AMainCharacter::AimOffset(float DeltaSeconds)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if(Speed == 0.f && !bIsInAir) // standing still not jumping
	{
		FRotator CurrentAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation,StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurnInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurningInPlace(DeltaSeconds);
	}
	if(Speed >0.f || bIsInAir) // run or jump
	{
		StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurnInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	AO_Pitch= GetBaseAimRotation().Pitch;
}



void AMainCharacter::TurningInPlace(float DeltaTime)
{
	if(AO_Yaw > 90.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw < -90.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurnInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurnInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}


