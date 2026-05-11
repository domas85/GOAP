// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "ChillOutAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UChillOutAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	float CurrentChillTime;

protected:

	/* The decease multiplier for the disengage meter every second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MeterDecreaseMultiplier = 1.0;

	/* The minimum time amount it will look in the time curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 MinChillTime = 1;

	/* The maximum time amount it will look in the time curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 MaxChillTime = 10;

	/* Proportianaly the chanse for the random seconds to happen, the key amount should be same as the min and max chill time */
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TObjectPtr<UCurveFloat> TimeCurve;

	/* The chance for the theat to wait before moving to the player, if it is 0 it will never wait, 100 means there 1 out of 100 chance for it to wait */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 ChillTimeChance = 80;

public:

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

	virtual bool CheckDynamicPreconditions(const FWorldState& InState) const override;

	float GetCurveWeightedRandom(UCurveFloat* Curve, int32 SampleCount = 10);

};
