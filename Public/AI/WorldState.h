// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldState.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FWorldState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TMap<FString, int32> IntStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TMap<FString, float> FloatStates;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TMap<FString, bool> BoolStates;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TMap<FString, FVector> VectorStates;
	
	inline bool HasState(const FString& StateName) const
	{
		return IntStates.Contains(StateName) || FloatStates.Contains(StateName) || BoolStates.Contains(StateName) || VectorStates.Contains(StateName);
	}

	void SetState(const FString& StateName, int32 StateValue)
	{
		IntStates.Add(StateName, StateValue);
	}
	void SetState(const FString& StateName, float StateValue)
	{
		FloatStates.Add(StateName, StateValue);
	}
	void SetState(const FString& StateName, bool bStateValue)
	{
		BoolStates.Add(StateName, bStateValue);
	}
	void SetState(const FString& StateName, FVector StateValue)
	{
		VectorStates.Add(StateName, StateValue);
	}

	inline int32 GetInt(const FString& Key) const
	{
		const int32* Temp = IntStates.Find(Key);
		return Temp ? *Temp : 0;
	}
	inline float GetFloat(const FString& Key) const
	{
		const float* Temp = FloatStates.Find(Key);
		return Temp ? *Temp : 0.f;
	}
	inline bool GetBool(const FString& Key) const
	{
		const bool* Temp = BoolStates.Find(Key);
		return Temp ? *Temp : false;
	}

	inline FVector GetVector(const FString& Key) const
	{
		const FVector* Temp = VectorStates.Find(Key);
		return Temp ? *Temp : FVector::Zero();
	}

	void RemoveState(const FString& StateName)
	{
		if (IntStates.Contains(StateName))
		{
			IntStates.Remove(StateName);
		}
		else if (FloatStates.Contains(StateName))
		{
			FloatStates.Remove(StateName);
		}
		else if (BoolStates.Contains(StateName))
		{
			BoolStates.Remove(StateName);
		}
		else if (VectorStates.Contains(StateName))
		{
			VectorStates.Remove(StateName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trying to remove state that doesn't exist: %s"), *StateName);
		}
	}
	void PrintStates() const
	{
		UE_LOG(LogTemp, Log, TEXT("------States-------"));
		for (const TPair<FString, int32>& State : IntStates)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: %d"), *State.Key, State.Value);
		}
		for (const TPair<FString, float>& State : FloatStates)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: %f"), *State.Key, State.Value);
		}
		for (const TPair<FString, bool>& State : BoolStates)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: %hs"), *State.Key, State.Value ? "True" : "False");
		}
		for (const TPair<FString, FVector>& State : VectorStates)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: %s"), *State.Key, *State.Value.ToString());
		}
		UE_LOG(LogTemp, Log, TEXT("-------------------"));
	}
	float DistanceTo(const FWorldState& Other) const
	{
		float Distance = 0;

		for (const TPair<FString, int32>& State : Other.IntStates)
		{
			Distance += FMath::Abs((float)(GetInt(State.Key) - State.Value));
		}

		for (const TPair<FString, float>& State : Other.FloatStates)
		{
			Distance += FMath::Abs((float)(GetFloat(State.Key) - State.Value));
		}

		for (const TPair<FString, bool>& State : Other.BoolStates)
		{
			if (GetBool(State.Key) != State.Value) Distance += 1.f;
		}

		for (const TPair<FString, FVector>& State : Other.VectorStates)
		{
			Distance += FMath::Abs((GetVector(State.Key) - State.Value).Length());
		}

		return Distance;
	}

	inline bool Satisfies(const FWorldState& GoalState) const
	{
		for (const TPair<FString, int32>& Goal : GoalState.IntStates)
		{
			if (!IntStates.Contains(Goal.Key) || GetInt(Goal.Key) < Goal.Value) 
			{
				return false; // Goal not satisfied
			}
		}
		for (const TPair<FString, float>& Goal : GoalState.FloatStates)
		{
			if (!FloatStates.Contains(Goal.Key) || GetFloat(Goal.Key) < Goal.Value) 
			{
				return false; // Goal not satisfied
			}
		}
		for (const TPair<FString, bool>& Goal : GoalState.BoolStates)
		{
			if (!BoolStates.Contains(Goal.Key) || GetBool(Goal.Key) != Goal.Value)
			{
				return false; // Goal not satisfied
			}
		}
		for (const TPair<FString, FVector>& Goal : GoalState.VectorStates)
		{
			if (!VectorStates.Contains(Goal.Key) || GetVector(Goal.Key).Length() < Goal.Value.Length()) 
			{
				return false; // Goal not satisfied
			}
		}
		return true; // All goals are satisfied
	}

	uint64 GetHash() const
	{
		uint64 Hash = 0;

		for (const TPair<FString, int32>& State : IntStates)
			Hash = HashCombine(Hash, GetTypeHash(State.Key) ^ (State.Value * 31));

		for (auto& State : FloatStates)
			Hash = HashCombine(Hash, GetTypeHash(State.Key) ^ ((int32)(State.Value * 1000)));

		for (const TPair<FString, bool>& State : BoolStates)
			Hash = HashCombine(Hash, GetTypeHash(State.Key) ^ (State.Value ? 1 : 2));

		for (const TPair<FString, FVector>& State : VectorStates)
			Hash = HashCombine(Hash, GetTypeHash(State.Key) ^ ((int32)FMath::Abs(State.Value.Length())));

		return Hash;
	}

	void Reset()
	{
		IntStates.Empty();
		FloatStates.Empty();
		BoolStates.Empty();
		VectorStates.Empty();
	}

	inline bool IsEmpty() const { return IntStates.Num() <= 0 && FloatStates.Num() <= 0 && BoolStates.Num() <= 0 && VectorStates.Num() <= 0; }

	inline bool operator==(const FWorldState& Other) const
	{
		return IntStates.OrderIndependentCompareEqual(Other.IntStates) && 
			FloatStates.OrderIndependentCompareEqual(Other.FloatStates) && 
			BoolStates.OrderIndependentCompareEqual(Other.BoolStates) &&
			VectorStates.OrderIndependentCompareEqual(Other.VectorStates);
	}
};


/*

class  WorldStates
{
	FString Key;
	int32 Value;
};
class PROJECTLIMINAL_API WorldState
{
private:

public:
	WorldState();
	~WorldState();

	TMap<FString, int32> States;
	bool HasState(const FString& StateName) const;
	void AddState(const FString& StateName, int32 StateValue);
	void ModifyState(const FString& StateName, int32 NewValue);
	void RemoveState(const FString& StateName);
	int DistanceTo(const WorldState& Other) const;
	void PrintStates() const;
	bool DoesSatisfy(const WorldState& GoalState) const;
	inline bool IsEmpty() const { return States.Num() <= 0; }

	inline bool operator==(const WorldState& Other) const
	{
		return States.OrderIndependentCompareEqual(Other.States);
	}
};
 */
