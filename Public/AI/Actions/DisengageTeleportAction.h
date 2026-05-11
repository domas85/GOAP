// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Actions/UActionAbstract.h"
#include "DisengageTeleportAction.generated.h"

class UNavigationPath;
/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UDisengageTeleportAction : public UActionAbstract
{
	GENERATED_BODY()

private:

	FNavLocation RandomTeleportPoint;
	UNavigationPath* NavPath;
	bool bTeleportSuccess = false;

protected:

	/* The shortest teleport distance, meaning it can not teleport to a point that less then the value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MinimumTeleportDistance = 200.0f;

	/* The Radius around the Threat, it will use it to randomly choose a valid teleport point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float TeleportRadius = 9000.0f;

	/* Offset the chosen teleport point, recomended to offset by 50 in the z axis to prevent teleporting in the ground */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FVector TeleportLocationOffset = FVector(0.0f, 0.0f, 50.0f);

	/* The Sound to play when the threat teleports */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TObjectPtr<USoundBase> TeleportSFX;

public:

	virtual void Start(ACharacter* InCharacter, FWorldState& InState) override;

	virtual void Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime) override;

	virtual bool IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const override;

	virtual void End(ACharacter* InCharacter, FWorldState& InState) override;

	virtual void CancelAction(ACharacter* InCharacter, FWorldState& InState) override;

	void RotateToClosestPlayer(ACharacter* InCharacter);

};
