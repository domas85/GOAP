// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "GetClosestPlayerAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UGetClosestPlayerAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	bool bPlayerFound = false;
	float DelayTimer = 0.0f;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float DelayAmount = 1.0f;

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

	int32 GetClosestPlayer(ACharacter* InCharacter) const;
};
