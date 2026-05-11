// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Planner.h"
#include "AI/GoapAgentBase.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheGoapComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldStateChanged, FString, KeyChanged);

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECTLIMINAL_API UTheGoapComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	bool bIsEnabled = false;

	bool bActionStarted = false;

	TObjectPtr<ACharacter> AgentCharacter;

public:

	// Sets default values for this component's properties
	UTheGoapComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bShouldDebugDraw = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	FWorldState CurrentWorldState;

	Planner GoapPlanner;

	TArray<TObjectPtr<UActionAbstract>> CurrentPlan;

	UPROPERTY(BlueprintCallable)
	FOnWorldStateChanged WorldStateChanged;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle GoalEvalTimerHandle;

	bool bGoalPriorityDirty = false;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnWorldStateChanged(FString InKey);

	void ReEvaluateGoals();

	bool ShouldReplan();

	void Replan();

	// Set World State
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateWorldStateInteger(FString InKey, int32 InValue);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateWorldStateFloat(FString InKey, float InValue);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateWorldStateBool(FString InKey, bool bInValue);

	// Get World State
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	int32 GetWorldStateInteger(FString InKey);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	float GetWorldStateFloat(FString InKey);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	bool GetWorldStateBool(FString InKey);

	void PlannerTick(float DeltaTime);

	// Debug Functions
	UFUNCTION(BlueprintCallable, Category = "AI")
	void DebugDrawState();
	UFUNCTION(BlueprintCallable, Category = "AI")
	void DebugDrawString(const UActionAbstract* Action);

	// Enable/Disable Planner
	inline void SetEnabled(bool bNewEnabled) { bIsEnabled = bNewEnabled; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	inline bool IsEnabled() const { return bIsEnabled; };

	UFUNCTION(BlueprintCallable, Category = "AI")
	void CleanUpPlan();
};
