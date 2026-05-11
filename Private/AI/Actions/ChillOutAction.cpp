// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/ChillOutAction.h"
#include "GameFramework/Character.h"

void UChillOutAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentChillTime = 0.0f;

	int RanInt = FMath::RandRange(1, 100 / ChillTimeChance);
	if (RanInt == 1)
	{
		CurrentChillTime = GetCurveWeightedRandom(TimeCurve, MaxChillTime);
	}
}

void UChillOutAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	// Decrease DisengageMeter when the Agent is chasing someone
	float CurrentMeter = InState.GetFloat("DisengageMeter");
	CurrentMeter -= MeterDecreaseMultiplier * InDeltaTime;
	CurrentMeter = FMath::Clamp(CurrentMeter, 0.0f, 100.0f);
	InState.SetState("DisengageMeter", CurrentMeter);

	// Chill Time: The Agent should wait for a random amount of time before moving
	CurrentChillTime -= InDeltaTime;
	CurrentChillTime = FMath::Clamp(CurrentChillTime, 0.0f, MaxChillTime);

	FString DebugMes = FString::Printf(TEXT("Chill time ends in: %.2f"), CurrentChillTime);

	DebugDrawStringSimple(InCharacter->GetActorLocation(), DebugMes, FColor::Yellow, 1);
}

bool UChillOutAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	if(CurrentChillTime <= 0.0f)
	{
		return true;
	}

	return false;
}

void UChillOutAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);
}

void UChillOutAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentChillTime = 0.0f;
	InState.SetState("ChillDone", false);
}

bool UChillOutAction::CheckDynamicPreconditions(const FWorldState& InState) const
{
	if ((!InState.GetBool("DansaPausaOn")))
	{
		return true;
	}

	return false;
}

float UChillOutAction::GetCurveWeightedRandom(UCurveFloat* Curve, int32 SampleCount)
{
	if (!Curve) return 0.f;

	// Store cumulative weights
	TArray<float> Weights;
	Weights.Reserve(SampleCount);

	float TotalWeight = 0.f;

	// Sample curve and accumulate
	for (int32 i = MinChillTime; i < SampleCount; i++)
	{
		float W = Curve->GetFloatValue(i);

		TotalWeight += W;
		Weights.Add(W);
	}

	// Roll random number
	float Roll = FMath::FRandRange(0.f, TotalWeight);
	float CumulativeChance = 0;

	// Find which index this roll corresponds to
	for (int32 i = MinChillTime; i < SampleCount; i++)
	{
		CumulativeChance += Weights[i - MinChillTime];

		if (Roll <= CumulativeChance)
		{
			return i;
		}
	}

	return 1.f;
}