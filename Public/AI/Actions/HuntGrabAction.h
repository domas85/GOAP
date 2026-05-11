// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "HuntGrabAction.generated.h"

class ALiminalPlayerController;
class ALiminalPlayerCharacter;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UHuntGrabAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	TObjectPtr<ALiminalPlayerCharacter> PlayerCharacter;
	TObjectPtr<ALiminalPlayerController> PlayerController;
	float CurrentDeathTimer = 0;

protected:

	/* How Long does the threat need to hold the player to kill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ClampMin = "0", UIMin = "0"))
	float DeathTime = 10;

	/* Offset where the players look direction will gravitate to, it is offset from the threats origin point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FVector LookAtOffset = FVector(0.0f, 0.0f, 250.0f);

	/* Look at force/influece, how hard it is to look away from the threat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float LookAtInfluence = 3.0f;

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

	inline int32 GetRequiredClicks(const FWorldState& InState) const
	{
		double GrabStacks = InState.GetInt("GrabStacks");
		double Floor = InState.GetInt("Floor");
		return FMath::Pow(Floor + 1, GrabStacks);
	}

};
