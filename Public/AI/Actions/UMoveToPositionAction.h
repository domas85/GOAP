// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UActionAbstract.h"
#include "CoreMinimal.h"
#include "UMoveToPositionAction.generated.h"

class AAIController;

/**
 *
 */
UCLASS()
class PROJECTLIMINAL_API UMoveToPositionAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	AActor* TargetActor;
	AAIController* Controller;

protected:

	/* The Distance before The threat grabs the Target Player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float DistanceToTarget = 500.0f;

	/* The decease multiplier for the disengage meter every second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MeterDecreaseMultiplier = 1.0;

public:

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

	virtual bool CheckDynamicPreconditions(const FWorldState& InState) const override;

};
