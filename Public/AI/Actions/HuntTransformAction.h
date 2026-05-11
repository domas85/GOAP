// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "HuntTransformAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UHuntTransformAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	float HuntDelay = 0.f;

protected:

	/* The delay before the threat starts hunting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ClampMin = "0", UIMin = "0"))
	float DelayTime = 5;

public:

	virtual void Start(ACharacter* InCharacter, FWorldState& InState) override;

	virtual void Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime) override;

	virtual bool IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const override;

	virtual void End(ACharacter* InCharacter, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

};
