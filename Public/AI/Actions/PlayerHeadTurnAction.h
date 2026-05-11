// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "PlayerHeadTurnAction.generated.h"

class ALiminalPlayerController;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UPlayerHeadTurnAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	TObjectPtr<ALiminalPlayerController> PlayerController;
	FRotator PlayerLookRotation;
	FRotator DesiredLookAtRotation;
	float CurrentRotationTime;

protected:

	/* The class for the camera shake it will use for the transition torwards the threat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	/* How fast it will force the Player to look at the threat, camera shake might be less visible if it is too fast */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float CameraTurnSpeedMultiplier = 1.0f;

	/* Head turn offset, it is offset from the threats origin point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FVector LookAtOffset = FVector(0.0f, 0.0f, 320.0f);

	/* The Sound to play when the player turns their head */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TObjectPtr<USoundBase> HeadTurnSFX;

public:
	
	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

};
