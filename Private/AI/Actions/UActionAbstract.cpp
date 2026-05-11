// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/UActionAbstract.h"

UActionAbstract::UActionAbstract()
{
}

UActionAbstract::UActionAbstract(int32 InCost) : _Cost(InCost)
{
}

bool UActionAbstract::CheckPreconditions(const FWorldState& InWorldState)
{
	if (!InWorldState.Satisfies(_Preconditions))
	{
		return false;
	}

	return CheckDynamicPreconditions(InWorldState); // All preconditions met if there is no special Dynamic precondition
}

void UActionAbstract::ApplyEffects(FWorldState& InWorldState) const
{
	for (TPair<FString, int32> Effect : _Effects.IntStates)
	{
		InWorldState.SetState(Effect.Key, (int32)Effect.Value);
	}
	for (TPair<FString, float> Effect : _Effects.FloatStates)
	{
		InWorldState.SetState(Effect.Key, (float)Effect.Value);
	}
	for (TPair<FString, bool> Effect : _Effects.BoolStates)
	{
		InWorldState.SetState(Effect.Key, (bool)Effect.Value);
	}
	for (TPair<FString, FVector> Effect : _Effects.VectorStates)
	{
		InWorldState.SetState(Effect.Key, (FVector)Effect.Value);
	}
}

UActionAbstract::~UActionAbstract()
{
	_Preconditions.Reset();
	_Effects.Reset();
}

void UActionAbstract::DebugDrawStringSimple(FVector InLocation, FString InDebugText, FColor InColor, int32 InDrawCount) const
{
	DrawDebugString(
		GetWorld(),
		InLocation + FVector(0, 0, 100 + (InDrawCount * 50)),	// Draw Location
		InDebugText,											// Display the name of the current action
		nullptr,												// No attached actor
		InColor,												// Text color
		0.f,													// Duration 
		true													// Draw shadow for better visibility
	);
}