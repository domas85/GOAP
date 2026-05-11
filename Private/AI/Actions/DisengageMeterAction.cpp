// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/DisengageMeterAction.h"
#include "AI/GoapAgentBase.h"

void UDisengageMeterAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	DisengageAmount = InState.GetFloat("DisengageMeter");
}

void UDisengageMeterAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	DisengageAmount += InState.GetInt("PlayersLookingCount") * DisengageMultiplier * InDeltaTime;
	InState.SetState("DisengageMeter", DisengageAmount);

	FString DebugMes = FString::Printf(TEXT("Disengage Meter: %.2f"), DisengageAmount);

	DebugDrawStringSimple(InCharacter->GetActorLocation(), DebugMes, FColor::Yellow, 1);

}

bool UDisengageMeterAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	if (DisengageAmount >= 100)
	{
		return true;
	}
	return false;
}

void UDisengageMeterAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	// Implementation for ending the action, e.g., cleaning up any variables or states

	ApplyEffects(InState);
	DisengageAmount = 0.f;
}

void UDisengageMeterAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{

}
