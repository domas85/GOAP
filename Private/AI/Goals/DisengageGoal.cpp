// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Goals/DisengageGoal.h"

float UDisengageGoal::GetPriority(const FWorldState& InState)
{
	float PrioValue = 0;

	PrioValue += InState.GetBool("IsSeenByPlayer") ? 100.0f : 0.0f;

	PrioValue += InState.GetBool("DisengageTeleport") ? 150.0f : 0.0f;

	PrioValue -= InState.GetBool("IsCloseToPlayer") ? 50.0f : 0.0f;

	return PrioValue;
}