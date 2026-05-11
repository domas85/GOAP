// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "HuntChargeAction.generated.h"

class AAIController;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UHuntChargeAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	AActor* TargetActor;
	AAIController* Controller;
	float CurrentSpeed;
	float ThreatLookedAtMeter = 0.0f;
	float LookMeter = 0.0f;
	float Timer = 0.0f;

protected:

	/* The Distance before The threat grabs the Target Player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float DistanceToTarget = 300.0f;

	/* The Speed it will eventually reach during the chase */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float HuntSpeed = 400.0f;

	/* The amount of time it will wait before increasing the "LookMeter" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float FirstLookGrace = 2.0f;

	/* How often should the "LookMeter" increase in seconds, it uses the LookMeterIncreaseAmount  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float LookSeconds = 0.5f;

	/* The amount the "LookMeter" should increase percentage wise */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100") , Category = "Properties")
	float LookMeterIncreaseAmount = 1.0f;

	/* This is how much it increases in speed to reach base Chase speed when Players are not looking at threat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"), Category = "Properties")
	float NotLookingSpeedIncrease = 1.0f;

public:

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

	float GetCurveWeightedRandom(UCurveFloat* Curve, int32 SampleCount = 10);
};