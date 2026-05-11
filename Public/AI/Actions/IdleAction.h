// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "IdleAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UIdleAction : public UActionAbstract
{
	GENERATED_BODY()

public:

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;
};
