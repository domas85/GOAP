// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"

/**
 *
 */
class PROJECTLIMINAL_API Node
{
public:
	
	Node();
	Node(FWorldState InState,TObjectPtr<UActionAbstract> InAction, int32 InG, int32 InH, int32 InParentId);
	~Node();
	
	FWorldState State;
	TObjectPtr<UActionAbstract> Action;
	int32 ParentIndex = INDEX_NONE;

	int32 Gcost = 0;
	int32 Hcost = 0;
	
	inline int32 Fcost() const { return Gcost + Hcost; }

	inline bool operator==(const Node& Other) const
	{
		return this->State == Other.State;
	}	
};
