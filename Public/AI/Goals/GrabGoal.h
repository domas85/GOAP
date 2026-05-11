// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Goals/GoalAbstract.h"
#include "GrabGoal.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UGrabGoal : public UGoalAbstract
{
	GENERATED_BODY()
	
	virtual float GetPriority(const FWorldState& InState);
};
