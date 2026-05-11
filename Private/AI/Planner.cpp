// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Planner.h"
#include "AI/Actions/IdleAction.h"

Planner::Planner()
{
}

Planner::~Planner()
{
	OpenList.Empty();
	ClosedSet.Empty();
}

int32 Planner::PopBestNode()
{
	OpenList.Sort([this](int32 A, int32 B)
		{
			return Nodes[A].Fcost() < Nodes[B].Fcost();
		});

	int32 BestIndex = OpenList[0];
	OpenList.RemoveAt(0);

	ClosedSet.Add(Nodes[BestIndex].State.GetHash());
	return BestIndex;
}

TArray<TObjectPtr<UActionAbstract>> Planner::Plan(FWorldState& InState, const FWorldState& Goal, const TArray<TObjectPtr<UActionAbstract>>& AvailableActions)
{
	// Reset planner
	Nodes.Reset();
	OpenList.Reset();
	ClosedSet.Reset();

	//Start Node
	Node Root = Node(InState, nullptr, 0, InState.DistanceTo(Goal), INDEX_NONE);

	int32 RootIndex = Nodes.Add(MoveTemp(Root));
	OpenList.Add(RootIndex);

	while (OpenList.Num() > 0)
	{
		int32 CurrentNodeIndex = PopBestNode();
		Node& CurrentNode = Nodes[CurrentNodeIndex];
		if (CurrentNode.State.Satisfies(Goal))
		{
			TArray<TObjectPtr<UActionAbstract>> Plan;
			int32 Trace = CurrentNodeIndex;

			while (Trace != INDEX_NONE)
			{
				const Node& ThisNode = Nodes[Trace];
				if (ThisNode.Action != nullptr)
				{
					Plan.Insert(ThisNode.Action, 0);
				}

				Trace = ThisNode.ParentIndex;
			}

			return Plan;
		}
		for (const TObjectPtr<UActionAbstract>& Action : AvailableActions)
		{
			if (Action == NULL)
			{
				UE_LOG(LogTemp, Warning, TEXT("Action is null in the Available Actions continuing"));
				continue;
			}

			if (Action->CheckPreconditions(CurrentNode.State))
			{
				FWorldState NewState = CurrentNode.State;
				Action->ApplyEffects(NewState);
				Node NewNode = Node(NewState, Action, CurrentNode.Gcost + Action->ActionCost(), NewState.DistanceTo(Goal), CurrentNodeIndex);
				int64 StateHash = NewState.GetHash();
				if (ClosedSet.Contains(StateHash))
				{
					continue; // Skip already evaluated states
				}
				int32 ExistingNodeIndex = OpenList.IndexOfByPredicate([&](int32 Index)
					{
						return Nodes[Index].State == NewState;
					});
				if (ExistingNodeIndex != INDEX_NONE)
				{
					if (NewNode.Gcost < Nodes[ExistingNodeIndex].Gcost)
					{
						Nodes[ExistingNodeIndex] = MoveTemp(NewNode); // Update node with better path
					}
				}
				else
				{
					int32 NewNodeIndex = Nodes.Add(MoveTemp(NewNode));
					OpenList.Add(NewNodeIndex);
				}
			}
		}
	}

	TArray<TObjectPtr<UActionAbstract>> Temp;
	Temp.Add(AvailableActions[0]);
	return Temp;
}