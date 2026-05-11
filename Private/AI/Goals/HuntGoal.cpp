// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Goals/HuntGoal.h"

float UHuntGoal::GetPriority(const FWorldState& InState)
{
	float PrioValue = 0;

	PrioValue += InState.GetBool("HuntOn") ? 160.0f : 0.0f;

	return PrioValue;
}