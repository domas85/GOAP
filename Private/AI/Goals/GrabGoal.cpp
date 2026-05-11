// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Goals/GrabGoal.h"

float UGrabGoal::GetPriority(const FWorldState& InState)
{
	float PrioValue = 0;

	if (!InState.GetBool("IsCloseToPlayer"))
	{
		return PrioValue;
	}
	PrioValue += 80;


	PrioValue += InState.GetBool("IsSeenByPlayer") ? 50.0f : 0.0f;

	return PrioValue;
}