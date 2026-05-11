// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "HuntCooldownAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UHuntCooldownAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	float CurrentCooldownTime = 0;

protected:

	/* How Long the threat will be on cooldown after a failed hunt grab */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ClampMin = "0", UIMin = "0"))
	float CooldownTime = 3;

public:

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

};
