// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "DisengageMeterAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UDisengageMeterAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	float DisengageAmount = 0.f;

protected:

	/* Base multiplier on to of the count of players looking for the disengage meter. Example: DisengageMultiplier of 1 and 2 players looking means that the disengage increases by 2 each second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float DisengageMultiplier = 10.0f;

public:

	virtual void Start(ACharacter* InCharacter, FWorldState& InState) override;

	virtual void Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime) override;

	virtual bool IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const override;

	virtual void End(ACharacter* InCharacter, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;
};
