// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "KeepHoldAction.generated.h"

class ALiminalPlayerController;
class ALiminalPlayerCharacter;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UKeepHoldAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	TObjectPtr<ALiminalPlayerCharacter> PlayerCharacter;
	TObjectPtr<ALiminalPlayerController> PlayerController;
	float CurrentHoldTime = 0;
	FRotator OldRotation = FRotator(0,0,0);

protected:

	/* Offset where the players look direction will gravitate to, it is offset from the threats origin point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FVector LookAtOffset = FVector(0.0f, 0.0f, 320.0f);

	/* Look at force/influece, how hard it is to look away from the threat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float LookAtInfluence = 3.0f;

	/* Walk force/influece, how hard the Player walk towards the threat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float WalkInfluence = 3.0f;

	/* The Player will walk towards the threat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bShouldWalk = false;

	/* The amount of time it takes for the grab to complete, Making the Player blind and slowed for a few seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float GrabTime = 10.0f;

	/* The distance you need to be from the threat for the strong grab to trigger (Keep in mind that for RoseMan if you are right next to it, the value is aroun 87). 
	The distance from Player to Threat <= StrongGrabDistance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float StrongGrabDistance = 90.0f;

	/* The amount of grab stacks to apply on a successful strong grab */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 StrongGrabStackIncrease = 2;

	/* The distance the player need to walk away from the threat to break the hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float  DistanceToBreakHold = 800.0f;

	/* The angle the player need to look away from the threat to break the hold, it is the angle between the direction from player to threat and the players look direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ClampMin = "0", ClampMax = "180", UIMin = "0", UIMax = "180"))
	float AngleToBreakHold = 80.0f;

	/* The Sound to play throughout entrance, Volume is based on distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TObjectPtr<USoundBase> EntrancedSFX;

	/* When the player is closest to threat. The amount volume will be multiplied by distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float PlayerCloseVolumeMultiplier = 50;

	/* The distance the player has to be from the threat to hear the default volume multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float LowestVolumeDistance = 500;

public:

	virtual void Start(ACharacter* Character, FWorldState& InState) override;

	virtual void Tick(ACharacter* Character, FWorldState& InState, float DeltaTime) override;

	virtual bool IsActionComplete(ACharacter* Character, FWorldState& InState) const override;

	virtual void End(ACharacter* Character, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;
};
