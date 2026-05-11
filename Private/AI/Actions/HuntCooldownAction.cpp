// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/HuntCooldownAction.h"
#include "GameFramework/Character.h"

void UHuntCooldownAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentCooldownTime = CooldownTime;
}

void UHuntCooldownAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	CurrentCooldownTime -= InDeltaTime;
	CurrentCooldownTime = FMath::Max(CurrentCooldownTime, 0.f);

	DebugDrawStringSimple(InCharacter->GetActorLocation(), FString::Printf(TEXT("Cooldown: %.2f"), CurrentCooldownTime), FColor::Red, 1);
}

bool UHuntCooldownAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	return CurrentCooldownTime <= 0.f;
}

void UHuntCooldownAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);
}

void UHuntCooldownAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentCooldownTime = 0;
}