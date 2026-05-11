// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.h"

/**
 *
 */
class PROJECTLIMINAL_API Planner
{
private:
	TArray<Node> Nodes;
	TArray<int32> OpenList;
	TSet<int64> ClosedSet;

	int32 PopBestNode();

public:
	Planner();
	~Planner();

	TArray<TObjectPtr<UActionAbstract>> Plan(FWorldState& InState, const FWorldState& Goal, const TArray<TObjectPtr<UActionAbstract>>& AvailableActions);
};
