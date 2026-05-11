// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "GetTargetPlayerAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UGetTargetPlayerAction : public UActionAbstract
{
	GENERATED_BODY()
	
private:

	int32 TargetPlayer = -1;
	float DelayTime = 0;

public:

	/* How long it will way before continuing with other actions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float SearchDelay = 2;

	virtual void Start(ACharacter * InCharacter, FWorldState & InState) override;

	virtual void Tick(ACharacter * InCharacter, FWorldState & InState, float InDeltaTime) override;

	virtual bool IsActionComplete(ACharacter * InCharacter, FWorldState & InState) const override;

	virtual void End(ACharacter * InCharacter, FWorldState & InState) override;

	virtual void CancelAction(ACharacter * InCharacter, FWorldState & InState) override;
};
