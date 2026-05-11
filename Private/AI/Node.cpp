// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Node.h"

Node::Node() : Gcost(0), Hcost(0)
{
}

Node::Node(FWorldState InState,TObjectPtr<UActionAbstract> InAction, int32 InG, int32 InH, int32 InParentId)
	: State(InState), Action(MoveTemp(InAction)), ParentIndex(InParentId), Gcost(InG), Hcost(InH)
{
}

Node::~Node()
{
	Action = nullptr;
	State.IntStates.Reset();
	State.BoolStates.Reset();
}

