// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Actions/UActionAbstract.h"
#include "AI/Goals/GoalAbstract.h"	

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GoapAgentBase.generated.h"

class UTheGoapComponent;

/**
 *
 */
UCLASS()
class PROJECTLIMINAL_API AGoapAgentBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoapAgentBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, Category = "AI")
	TArray<TObjectPtr<UActionAbstract>> AvailableActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, Category = "AI")
	TArray<TObjectPtr<UGoalAbstract>> AvailableGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "AI")
	TObjectPtr<UGoalAbstract> CurrentGoal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UTheGoapComponent> GoapComponent;

private:

	bool bPendingDestroy = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "AI")
	bool bIsGrabbing = false;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "AI")
	FVector PlayerPositon;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void EvaluateGoals(FWorldState& InState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	FBoxSphereBounds GetSkeletalMeshBounds(USkinnedAsset* InSkinnedAsset);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	inline FWorldState GetGoalState() { return CurrentGoal->GoalState; }

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	bool GetPendingDestroy() const { return bPendingDestroy; }

	UFUNCTION(BlueprintCallable, Category = "AI")
	void PendingDestroy() { bPendingDestroy = true; }

public:
	// Effect Triggers

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "AI")
	void ServerSetGrab(bool InGrab);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "AI")
	void ServerUpdateTargetPlayerPosition(FVector InPosition);

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable, Category = "AI")
	void Multicast_PlaySoundAtAgentLocation(USoundBase* InSFX);

};
