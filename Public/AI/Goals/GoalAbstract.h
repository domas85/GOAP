// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/WorldState.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalAbstract.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class PROJECTLIMINAL_API UGoalAbstract : public UObject
{
	GENERATED_BODY()
	
public:
	UGoalAbstract();
	virtual ~UGoalAbstract();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FWorldState GoalState;

	virtual float GetPriority(const FWorldState& InState) PURE_VIRTUAL(UGoalAbstract::GetPriority, return 0;);
};
