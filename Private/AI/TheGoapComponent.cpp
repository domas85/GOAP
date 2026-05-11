// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/TheGoapComponent.h"

// Sets default values for this component's properties
UTheGoapComponent::UTheGoapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UTheGoapComponent::BeginPlay()
{
	Super::BeginPlay();

	// Init Agent
	AgentCharacter = Cast<ACharacter>(GetOwner());

	GoapPlanner = Planner();

	ReEvaluateGoals();

	WorldStateChanged.AddDynamic(this, &UTheGoapComponent::OnWorldStateChanged);

	GetWorld()->GetTimerManager().SetTimer(
		GoalEvalTimerHandle,
		this,
		&UTheGoapComponent::ReEvaluateGoals,
		0.25f,
		true
	);
}


// Called every frame
void UTheGoapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...

	PlannerTick(DeltaTime);
}

void UTheGoapComponent::PlannerTick(float DeltaTime)
{

	if (!GetOwner()->HasAuthority()) return;

	if (bGoalPriorityDirty)
	{
		bGoalPriorityDirty = false;
		ReEvaluateGoals();
		Replan();
	}

	if (CurrentPlan.IsEmpty() && bIsEnabled)
	{
		CurrentPlan = GoapPlanner.Plan(CurrentWorldState, Cast<AGoapAgentBase>(AgentCharacter)->GetGoalState(), Cast<AGoapAgentBase>(AgentCharacter)->AvailableActions);
	}

	if (!CurrentPlan.IsEmpty())
	{
		UActionAbstract* CurrentAction = CurrentPlan[0];

		if (!bActionStarted)
		{
			CurrentAction->Start(AgentCharacter, CurrentWorldState);
			bActionStarted = true;
		}
		CurrentAction->Tick(AgentCharacter, CurrentWorldState, DeltaTime);

		if (CurrentAction->IsActionComplete(AgentCharacter, CurrentWorldState))
		{
			CurrentAction->End(AgentCharacter, CurrentWorldState);
			CurrentPlan.RemoveAt(0);
			bActionStarted = false;
		}

		if (!bIsEnabled)
		{
			CleanUpPlan();
		}
	}

	if (ShouldReplan())
	{
		UE_LOG(LogTemp, Log, TEXT("Re-Planning "));

		Replan();
	}

	if (bShouldDebugDraw && bIsEnabled)
	{
		if (!CurrentPlan.IsEmpty())
		{
			UActionAbstract* CurrentAction = CurrentPlan[0];
			DebugDrawString(CurrentAction);
		}

		DebugDrawState();
	}
}

bool UTheGoapComponent::ShouldReplan()
{
	// No plan to replan
	if (CurrentPlan.IsEmpty()) return false;

	// If the World State changes so it can not perform the action anymore it should replan
	if (!CurrentPlan[0]->CheckPreconditions(CurrentWorldState))	return true;

	// If the World State satisfies the goal then it should replan
	if (CurrentWorldState.Satisfies(Cast<AGoapAgentBase>(GetOwner())->GetGoalState())) return true;

	return false;
}

void UTheGoapComponent::Replan()
{
	if (!CurrentPlan.IsEmpty())
	{
		UActionAbstract* CurrentAction = CurrentPlan[0];
		CurrentAction->CancelAction(AgentCharacter, CurrentWorldState);
		CurrentPlan.Reset();
		bActionStarted = false;
		CurrentPlan = GoapPlanner.Plan(CurrentWorldState, Cast<AGoapAgentBase>(AgentCharacter)->GetGoalState(), Cast<AGoapAgentBase>(AgentCharacter)->AvailableActions);
	}
}

void UTheGoapComponent::UpdateWorldStateInteger(FString InKey, int32 InValue)
{
	CurrentWorldState.SetState(InKey, InValue);
}

void UTheGoapComponent::UpdateWorldStateFloat(FString InKey, float InValue)
{
	CurrentWorldState.SetState(InKey, InValue);
}

void UTheGoapComponent::UpdateWorldStateBool(FString InKey, bool bInValue)
{
	CurrentWorldState.SetState(InKey, bInValue);
}

int32 UTheGoapComponent::GetWorldStateInteger(FString InKey)
{
	if (CurrentWorldState.IntStates.Contains(InKey))
	{
		return CurrentWorldState.GetInt(InKey);
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find state returning default values."));
	return 0;
}

float UTheGoapComponent::GetWorldStateFloat(FString InKey)
{
	if (CurrentWorldState.FloatStates.Contains(InKey))
	{
		return CurrentWorldState.GetFloat(InKey);
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find state returning default values."));
	return 0;
}

bool UTheGoapComponent::GetWorldStateBool(FString InKey)
{
	if (CurrentWorldState.BoolStates.Contains(InKey))
	{
		return CurrentWorldState.GetBool(InKey);
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find state returning default values."));
	return 0;
}

void UTheGoapComponent::OnWorldStateChanged(FString InKey)
{
	bGoalPriorityDirty = true;
}

void UTheGoapComponent::ReEvaluateGoals()
{
	Cast<AGoapAgentBase>(AgentCharacter)->EvaluateGoals(CurrentWorldState);
}

void UTheGoapComponent::DebugDrawString(const UActionAbstract* Action)
{
	if (IsValid(Action))
	{
		DrawDebugString(
			GetWorld(),
			AgentCharacter->GetActorLocation() + FVector(0, 0, 100),	// Draw above the character
			"Action: " + Action->GetName(),								// Display the name of the current action
			nullptr,													// No attached actor
			FColor::Yellow,												// Text color
			0.f,														// Duration 
			true														// Draw shadow for better visibility
		);
	}
}

void UTheGoapComponent::DebugDrawState()
{
	FString Action = "None/Idle";

	if (!CurrentPlan.IsEmpty())
	{
		Action = CurrentPlan[0]->GetName();
	}

	TObjectPtr<UGoalAbstract> Goal = Cast<AGoapAgentBase>(AgentCharacter)->CurrentGoal;
	FString DebugText = "Goal: " + (IsValid(Goal) ? Goal->GetName() : "NULL") + "\n";
	DebugText += "Goal Priority: " + FString::SanitizeFloat(Cast<AGoapAgentBase>(AgentCharacter)->CurrentGoal->GetPriority(CurrentWorldState)) + "\n";
	DebugText += "Current Action: " + Action + "\n";
	DebugText += "World State:\n";

	for (auto& State : CurrentWorldState.IntStates)
	{
		DebugText += FString::Printf(TEXT("%s = %d\n"),
			*State.Key,
			State.Value);
	}
	for (auto& State : CurrentWorldState.FloatStates)
	{
		DebugText += FString::Printf(TEXT("%s = %f\n"),
			*State.Key,
			State.Value);
	}
	for (auto& State : CurrentWorldState.BoolStates)
	{
		DebugText += FString::Printf(TEXT("%s = %s\n"),
			*State.Key,
			State.Value ? TEXT("true") : TEXT("false"));
	}

	GEngine->AddOnScreenDebugMessage(
		(uint64)((PTRINT)this),
		0.f, FColor::Green, DebugText
	);
}

void UTheGoapComponent::CleanUpPlan()
{
	if (!CurrentPlan.IsEmpty())
	{
		UActionAbstract* CurrentAction = CurrentPlan[0];
		CurrentAction->CancelAction(AgentCharacter, CurrentWorldState);
		CurrentPlan.Reset();
		bActionStarted = false;
	}
}

