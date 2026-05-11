// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/HuntTransformAction.h"
#include "Core/LiminalGameMode.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Core/LiminalPlayerController.h"

void UHuntTransformAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	HuntDelay = DelayTime;
}

void UHuntTransformAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	HuntDelay -= InDeltaTime;

	FString DebugMes = FString::Printf(TEXT("Transformation done in: %.2f"), HuntDelay);

	DebugDrawStringSimple(InCharacter->GetActorLocation(), DebugMes, FColor::Yellow, 1);
}

bool UHuntTransformAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	if (HuntDelay <= 0)
	{
		return true;
	}
	return false;
}

void UHuntTransformAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);
}

void UHuntTransformAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	HuntDelay = DelayTime;
}

