// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/IdleAction.h"

void UIdleAction::Start(ACharacter* InCharacter, FWorldState& InState)
{

}

void UIdleAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{

}

bool UIdleAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	return true;
}

void UIdleAction::End(ACharacter* InCharacter, FWorldState& InState)
{

}

void UIdleAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{

}
