// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/UMoveToPositionAction.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/Character.h"


void UMoveToPositionAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	// Implementation for starting the action, e.g., setting up any necessary variables or states
	Controller = Cast<AAIController>(InCharacter->GetController());

	TargetActor = UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), InState.GetInt("PlayerID"));

}

void UMoveToPositionAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	// Decrease DisengageMeter when the Agent is chasing someone
	float CurrentMeter = InState.GetFloat("DisengageMeter");
	CurrentMeter -= MeterDecreaseMultiplier * InDeltaTime;
	CurrentMeter = FMath::Clamp(CurrentMeter, 0.0f, 100.0f);
	InState.SetState("DisengageMeter", CurrentMeter);

	if (IsValid(TargetActor) && IsValid(Controller))
	{
		FVector Location = TargetActor->GetActorLocation();
		Controller->MoveToLocation(Location);
	}
}

bool UMoveToPositionAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	// Check if the character is close enough to the target location
	FVector CharacterLocation = InCharacter->GetActorLocation();
	FVector TargetLocation = TargetActor ? TargetActor->GetActorLocation() : FVector::ZeroVector;
	bool bIsClose = FVector::Dist(CharacterLocation, TargetLocation) < DistanceToTarget;

	// Check if the character has line of sight to the target
	bool bHasLineOfSight = Controller ? Controller->LineOfSightTo(TargetActor, FVector(0, 0, 0)) : false;

	if (bIsClose && bHasLineOfSight)
	{
		return true;
	}

	return false;
}

void UMoveToPositionAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);

	if (IsValid(Controller))
	{
		Controller->StopMovement();
	}

	TargetActor = nullptr;
	InState.SetState("ChillDone", false);
}

void UMoveToPositionAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	if (IsValid(Controller))
	{
		Controller->StopMovement();
	}

	TargetActor = nullptr;
	InState.SetState("ChillDone", false);
}

bool UMoveToPositionAction::CheckDynamicPreconditions(const FWorldState& InState) const
{
	if ((InState.GetBool("DansaPausa")))
	{
		return true;
	}

	if ((!InState.GetBool("IsSeenByPlayer") && InState.GetBool("DansaPausaOn")))
	{
		return true;
	}

	if ((!InState.GetBool("IsSeenByPlayer") && InState.GetBool("ChillDone")))
	{
		return true;
	}

	return false;
}
