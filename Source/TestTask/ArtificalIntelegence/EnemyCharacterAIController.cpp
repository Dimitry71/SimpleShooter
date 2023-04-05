// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAIController.h"
#include "EnemyAICharacter.h"
#include "AIController.h"
#include "TestTask/Character/MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "TestTask/Components/HealthComponent.h"
#include "TestTask/Pickups/PickupSpawnPoint.h"
#include "Engine/TargetPoint.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyCharacterAIController::AEnemyCharacterAIController()
{
	AIBehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	AIBlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	
	if (AIPerceptionComponent)
	{
		// Add AIPerceptionComponent to controller components
		SetPerceptionComponent(*AIPerceptionComponent);

		// create configuration for sight our AI-Character
		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
		if (SightConfig)
		{
			// Configuration Settings
			SightConfig->SightRadius = 1000.0f;
			SightConfig->LoseSightRadius = 1200.0f;
			SightConfig->PeripheralVisionAngleDegrees = 90.0f;
			SightConfig->SetMaxAge(5.0f);
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

			// Add Sight configuration to AIPerceptionComponent
			AIPerceptionComponent->ConfigureSense(*SightConfig);
			AAIController::GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemyCharacterAIController::OnTargetDetected);
		}
	}
}

void AEnemyCharacterAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		AIBlackboardComponent->SetValueAsBool(FName("DetectedPlayer"), Stimulus.WasSuccessfullySensed());
		PlayerCharacter = Cast<AMainCharacter>(Actor);
		AIBlackboardComponent->SetValueAsObject(FName("PlayerCharacter"), PlayerCharacter);
		SetFocus(PlayerCharacter);
	}
	else
	{
		AIBlackboardComponent->ClearValue(FName("PlayerCharacter"));
		SetFocus(nullptr);
	}
}

void AEnemyCharacterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	ChooseNavigationPoint();
}

void AEnemyCharacterAIController::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemyCharacterAIController::ChooseNavigationPoint()
{
	
	TArray<AActor*> NavigationPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), NavigationPoints);
	
	if(NavigationPoints.Num()> 0 )
	{
		int32 NavigationIndex = FMath::RandRange(0, NavigationPoints.Num() - 1);
		ATargetPoint* NavigationPoint = Cast<ATargetPoint>(NavigationPoints[NavigationIndex]);
		if (NavigationPoint)
		{
			AIBlackboardComponent->SetValueAsObject("NavigationPoint", NavigationPoint);
			MoveToActor(NavigationPoint, 5.0f, false, true);
			AIBlackboardComponent->SetValueAsBool("IsMoving", true);
		}
	}
}

void AEnemyCharacterAIController::ChooseNearAmmoPickup()
{
	AEnemyAICharacter* EnemyCharacter = Cast<AEnemyAICharacter>(GetPawn());
	TArray<AActor*> AmmoActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupSpawnPoint::StaticClass(), AmmoActors);
	if (AmmoActors.Num() > 0)
	{
		APickupSpawnPoint* BestAmmoActor = nullptr;
		float BestAmmoActorDistance = TNumericLimits<float>::Max();

		for (AActor* AmmoActor : AmmoActors)
		{
			APickupSpawnPoint* MyAmmoActor = Cast<APickupSpawnPoint>(AmmoActor);
			if (MyAmmoActor)
			{
				float AmmoActorDistance = FVector::Dist(EnemyCharacter->GetActorLocation(), AmmoActor->GetActorLocation());
				if (AmmoActorDistance < BestAmmoActorDistance)
				{
					BestAmmoActor = MyAmmoActor;
					BestAmmoActorDistance = AmmoActorDistance;
				}
			}
		}

		if (BestAmmoActor)
		{
			AIBlackboardComponent->SetValueAsObject("AmmoActor", BestAmmoActor);
			MoveToActor(BestAmmoActor, 5.0f, false, true);
			AIBlackboardComponent->SetValueAsBool("IsMoving", true);
		}
	}
}


void AEnemyCharacterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AEnemyAICharacter* EnemyCharacter = Cast<AEnemyAICharacter>(InPawn);
	AAIController* Controller = Cast<AAIController>(this);
	//Controller->OnMoveCompleted.AddDynamic(this, &AEnemyCharacterAIController::OnMoveCompleted);
	MovementComponent = GetCharacter()->GetCharacterMovement();
	if (EnemyCharacter && EnemyCharacter->GetBehaviorTree())
	{
		AIBlackboardComponent->InitializeBlackboard(*(EnemyCharacter->GetBehaviorTree()->BlackboardAsset));
		AIBehaviorTreeComponent->StartTree(*EnemyCharacter->GetBehaviorTree());
	}
	
}



void AEnemyCharacterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AEnemyAICharacter* EnemyCharacter = Cast<AEnemyAICharacter>(GetPawn());
	// if (EnemyCharacter->Health->IsElimmed())
	// {
	// 	// Если у врага закончились жизни, отключаем ИИ
	// 	AIBehaviorTreeComponent->StopTree();
	// 	StopMovement();
	// 	EnemyCharacter->SetLifeSpan(5.0f);
	// 	return;
	// }
	
	if(PlayerCharacter)
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
	}
	bool SencedPlayer = IsValid(GetBlackboardComponent()->GetValueAsObject(FName("PlayerCharacter")));
	UE_LOG(LogTemp, Warning, TEXT("SencedPLayer %s"), SencedPlayer ? TEXT("True") :TEXT("False"));

	
	if (SencedPlayer &&  EnemyCharacter->GetCurrentAmmo() > 0)
	{
		
		AIBlackboardComponent->SetValueAsVector("PlayerLocation", PlayerLocation);
		// Если можем атаковать игрока и есть достаточно боезапаса - атакуем
		bool bCanShoot = EnemyCharacter->CanFire();
		if (bCanShoot)
		{
			EnemyCharacter->Fire();
			EnemyCharacter->GetEquippedWeapon()->Fire(PlayerLocation);
			AIBlackboardComponent->SetValueAsBool("IsMoving", true);
		}
		return;
	}
	
	
	else if(!EnemyCharacter->IsFull() && !SencedPlayer)
	{
		ChooseNearAmmoPickup();
		return;
	}

	if (EnemyCharacter->IsFull() && !SencedPlayer)
	{
		if(AIBlackboardComponent && AIBlackboardComponent->GetValueAsBool("IsMoving"))
		{
			return;
		}
		ChooseNavigationPoint();
		return;
	}
	

	AIBlackboardComponent->SetValueAsBool("IsMoving", false);
}
