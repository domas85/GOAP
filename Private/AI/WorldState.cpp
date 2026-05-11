// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WorldState.h"

/*
WorldState::WorldState()
{
	States = TMap<FString, int32>();
}

bool WorldState::HasState(const FString& StateName) const
{
	return States.Contains(StateName);
}

void WorldState::AddState(const FString& StateName, int32 StateValue)
{
	States.Add(StateName, StateValue);
}

void WorldState::ModifyState(const FString& StateName, int32 NewValue)
{
	if (States.Contains(StateName))
	{
		States[StateName] = NewValue;
	}
}

void WorldState::RemoveState(const FString& StateName)
{
	States.Remove(StateName);
}

int WorldState::DistanceTo(const WorldState& Other) const
{
	int Distance = 0;
	for (const auto& State : States)
	{
		if (Other.States.Contains(State.Key))
		{
			Distance += FMath::Abs(State.Value - Other.States[State.Key]);
		}
		else
		{
			Distance += State.Value;
		}
	}
	for (const auto& State : Other.States)
	{
		if (!States.Contains(State.Key))
		{
			Distance += State.Value; 
		}
	}
	return Distance;
}

bool WorldState::DoesSatisfy(const WorldState& GoalState) const
{
	for (const auto& Goal : GoalState.States)
	{
		if (!States.Contains(Goal.Key) || States[Goal.Key] != Goal.Value)
		{
			return false; // Goal not satisfied
		}
	}
	return true; // All goals are satisfied
}

void WorldState::PrintStates() const
{
	for (const auto& State : States)
	{
		UE_LOG(LogTemp, Log, TEXT("%s: %d"), *State.Key, State.Value);
	}
}

WorldState::~WorldState()
{
	States.Empty();
}

*/