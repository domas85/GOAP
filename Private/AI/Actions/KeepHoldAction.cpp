// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/KeepHoldAction.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/GoapAgentBase.h"
#include "Core/LiminalPlayerController.h"
#include "Core/LiminalPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UKeepHoldAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentHoldTime = 0;
	PlayerCharacter = Cast<ALiminalPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), InState.GetInt("PlayerID")));
	PlayerController = Cast<ALiminalPlayerController>(PlayerCharacter->GetController());

	float LookingInfluence = LookAtInfluence * InState.GetInt("GrabStacks");
	float WalkingInfluence = WalkInfluence * InState.GetInt("GrabStacks");

	PlayerController->Client_UpdateTarget(InCharacter->GetActorLocation() + LookAtOffset, LookingInfluence, WalkingInfluence, bShouldWalk);
	PlayerCharacter->ClientReduceSpeedPercentage(60);
	PlayerCharacter->ClientEntrancedOn();
	PlayerController->ClientSpawnSound2D(EntrancedSFX);
	PlayerController->ClientSetThreatSoundDistances(LowestVolumeDistance, PlayerCloseVolumeMultiplier);

	Cast<AGoapAgentBase>(InCharacter)->ServerSetGrab(true);
}

void UKeepHoldAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	CurrentHoldTime += InDeltaTime;

	// Rotate Agent to Player 

	FVector AgentLocation = InCharacter->GetActorLocation();
	FVector PlayerLocation = Cast<APawn>(PlayerController->GetPawn())->GetActorLocation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(AgentLocation, PlayerLocation);
	InCharacter->SetActorRotation(DesiredRotation);


	Cast<AGoapAgentBase>(InCharacter)->ServerUpdateTargetPlayerPosition(PlayerLocation);
	FString DebugMes = FString::Printf(TEXT("Grab Completed in: %.2f"), GrabTime - CurrentHoldTime);
	DebugDrawStringSimple(InCharacter->GetActorLocation(), DebugMes, FColor::Yellow, 1);
}

bool UKeepHoldAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	FVector AgentLocation = InCharacter->GetActorLocation();
	FVector PlayerLocation = Cast<APawn>(PlayerController->GetPawn())->GetActorLocation();
	float DistanceToPlayer = FVector::Dist(PlayerLocation, AgentLocation);

	GEngine->AddOnScreenDebugMessage(
		(uint64)((PTRINT)this),
		0.f, FColor::Green, "Distance to player: " + FString::SanitizeFloat(DistanceToPlayer)
	);

	FVector ToAgentVector = (AgentLocation - PlayerLocation).GetSafeNormal();
	float dotprod = FVector::DotProduct(ToAgentVector, Cast<APawn>(PlayerController->GetPawn())->GetActorForwardVector());
	float AngleFromThreat = FMath::Acos(dotprod);
	AngleFromThreat = FMath::RadiansToDegrees(AngleFromThreat);

	GEngine->AddOnScreenDebugMessage(
		(uint64)((PTRINT)this),
		0.f, FColor::Yellow, "Angle: " + FString::SanitizeFloat(AngleFromThreat)
	);

	if (DistanceToPlayer <= StrongGrabDistance)
	{
		PlayerCharacter->ClientBlind(12, true);

		int32 CurrentStacks = InState.GetInt("GrabStacks");

		InState.SetState("GrabStacks", CurrentStacks + StrongGrabStackIncrease);

		return true;
	}

	if (CurrentHoldTime >= GrabTime)
	{
		PlayerCharacter->ClientBlind(3, true);

		return true;
	}

	if (DistanceToPlayer > DistanceToBreakHold || AngleFromThreat >= AngleToBreakHold) return true;

	return false;
}

void UKeepHoldAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);

	Cast<AGoapAgentBase>(InCharacter)->ServerSetGrab(false);
	PlayerController->ClientStopSound2D();


	if (IsValid(PlayerController) && IsValid(PlayerCharacter))
	{
		PlayerCharacter->ClientAddSpeedPercentage(60);
		PlayerCharacter->ClientEntrancedOff();
		PlayerController->Client_UpdateTarget(FVector::ZeroVector, 0, 0, false);
	}

	PlayerCharacter = nullptr;
	PlayerController = nullptr;
}

void UKeepHoldAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	Cast<AGoapAgentBase>(InCharacter)->ServerSetGrab(false);

	if (IsValid(PlayerController) && IsValid(PlayerCharacter))
	{
		PlayerController->Client_UpdateTarget(FVector::ZeroVector, 0, 0, false);
		PlayerCharacter->ClientSetSpeedPercentage(100);
		PlayerCharacter->ClientEntrancedOff();
	}

	PlayerCharacter = nullptr;
	PlayerController = nullptr;
}