// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GoapAgentBase.h"
#include "AI/TheGoapComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoapAgentBase::AGoapAgentBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!IsValid(GoapComponent))
	{
		GoapComponent = CreateDefaultSubobject<UTheGoapComponent>(TEXT("GoapComponent"));
	}
}

// Called when the game starts or when spawned
void AGoapAgentBase::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AGoapAgentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGoapAgentBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGoapAgentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGoapAgentBase, bIsGrabbing);
	DOREPLIFETIME(AGoapAgentBase, PlayerPositon);
}

void AGoapAgentBase::EvaluateGoals(FWorldState& InState)
{
	float HighestValue = -FLT_MAX;
	TObjectPtr<UGoalAbstract> BestGoal;

	for (TObjectPtr<UGoalAbstract>& Goal : AvailableGoals)
	{
		const float PriorityValue = Goal->GetPriority(InState);

		if (PriorityValue >= HighestValue)
		{
			HighestValue = PriorityValue;
			BestGoal = Goal;
		}
	}

	if (IsValid(BestGoal))
	{
		CurrentGoal = BestGoal;
		return;
	}
}

FBoxSphereBounds AGoapAgentBase::GetSkeletalMeshBounds(USkinnedAsset* InSkinnedAsset)
{
	return InSkinnedAsset->GetBounds();
}

void AGoapAgentBase::SetEnabled(bool bEnabled)
{
	if (IsValid(GoapComponent))
	{
		GoapComponent->SetEnabled(bEnabled);
	}
}

void AGoapAgentBase::ServerSetGrab_Implementation(bool InGrab)
{
	bIsGrabbing = InGrab;
}

void AGoapAgentBase::ServerUpdateTargetPlayerPosition_Implementation(FVector InPosition)
{
	PlayerPositon = InPosition;
}

void AGoapAgentBase::Multicast_PlaySoundAtAgentLocation_Implementation(USoundBase* InSFX)
{
	if (IsValid(InSFX))
	{
		UGameplayStatics::PlaySoundAtLocation(this, InSFX, GetActorLocation());
	}
}