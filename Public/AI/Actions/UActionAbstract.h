// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/WorldState.h"

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UActionAbstract.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECTLIMINAL_API UActionAbstract : public UObject
{
	GENERATED_BODY()


public:
	UActionAbstract();
	UActionAbstract(int32 InCost);
	virtual ~UActionAbstract();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float _Cost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FWorldState _Preconditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FWorldState _Effects;

	// Execution flow functions that must be implemented by derived classes

	bool CheckPreconditions(const FWorldState& InWorldState);

	void ApplyEffects(FWorldState& InWorldState) const;

	virtual bool CheckDynamicPreconditions(const FWorldState& InState) const { return true; }

	virtual void Start(ACharacter* InCharacter, FWorldState& InState) PURE_VIRTUAL(UActionAbstract::Start, );

	virtual void Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime) PURE_VIRTUAL(UActionAbstract::Tick, );

	virtual bool IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const PURE_VIRTUAL(UActionAbstract::IsActionComplete, return false;);

	virtual void End(ACharacter* InCharacter, FWorldState& InState) PURE_VIRTUAL(UActionAbstract::End, );

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) PURE_VIRTUAL(UActionAbstract::End, );

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	inline int32 ActionCost() const { return _Cost; }

	void DebugDrawStringSimple(FVector InLocation, FString InDebugText, FColor InColor, int32 DrawCount) const;
};
