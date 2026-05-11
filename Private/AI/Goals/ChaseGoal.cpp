// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Goals/ChaseGoal.h"

float UChaseGoal::GetPriority(const FWorldState& InState)
{
	float PrioValue = 0;

	if (!InState.GetBool("GraceTimeDone"))
	{
		return 0;
	}

	PrioValue += InState.GetBool("HasTarget") ? 50.0f : 0.0f;
	PrioValue += InState.GetBool("DansaPausa") ? 80.0f : 0.0f;
	PrioValue += InState.GetBool("GraceTimeDone") ? 10.0f : 0.0f;

	return PrioValue;
}