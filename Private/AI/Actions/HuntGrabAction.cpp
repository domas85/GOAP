// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/HuntGrabAction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/LiminalPlayerController.h"
#include "Core/LiminalPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/GoapAgentBase.h"


void UHuntGrabAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentDeathTimer = 0;

	PlayerCharacter = Cast<ALiminalPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), InState.GetInt("PlayerID")));
	PlayerController = Cast<ALiminalPlayerController>(PlayerCharacter->GetController());

	float LookInfluence = LookAtInfluence * InState.GetInt("GrabStacks");
	float WalkInfluence = LookAtInfluence * InState.GetInt("GrabStacks");

	PlayerController->Client_UpdateTarget(InCharacter->GetActorLocation() + LookAtOffset, WalkInfluence, LookInfluence, true);
	PlayerController->ClientSpawnSound2D(EntrancedSFX);
	PlayerController->ClientSetThreatSoundDistances(LowestVolumeDistance, PlayerCloseVolumeMultiplier);
	PlayerCharacter->ClientSetInputEnabled(false);
	PlayerCharacter->ClientEntrancedOn();


	PlayerCharacter->ServerResetClickAmount(true, GetRequiredClicks(InState));

	Cast<AGoapAgentBase>(InCharacter)->ServerSetGrab(true);
}

void UHuntGrabAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	CurrentDeathTimer += InDeltaTime;

	float TimeForDeath = DeathTime - CurrentDeathTimer;

	FString DebugMes = FString::Printf(TEXT("Player will die in: %.2f"), TimeForDeath);

	DebugDrawStringSimple(InCharacter->GetActorLocation(), DebugMes, FColor::Red, 1);

	// Rotate Agent to Player 

	FVector AgentLocation = InCharacter->GetActorLocation();
	FVector PlayerLocation = Cast<APawn>(PlayerController->GetPawn())->GetActorLocation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(AgentLocation, PlayerLocation);
	InCharacter->SetActorRotation(DesiredRotation);

	Cast<AGoapAgentBase>(InCharacter)->ServerUpdateTargetPlayerPosition(PlayerLocation);
}

bool UHuntGrabAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	int32 ClicksRequiredToBreakHold = GetRequiredClicks(InState);
	FString DebugMes = "Clicks Needed to free Player: ";
	DebugMes.AppendInt(ClicksRequiredToBreakHold);

	DebugDrawStringSimple(InCharacter->GetActorLocation(), DebugMes, FColor::Blue, 2);


	if (PlayerCharacter->ClickAmount >= ClicksRequiredToBreakHold)
	{
		PlayerController->Client_UpdateTarget(FVector::ZeroVector, 0, 0, false);
		PlayerCharacter->ClientEntrancedOff();
		PlayerCharacter->ClientSetInputEnabled(true);
	

		return true;
	}

	if (CurrentDeathTimer >= DeathTime)
	{
		PlayerController->Client_UpdateTarget(FVector::ZeroVector, 0, 0, false);
		PlayerController->ClientSetPlayerIsDead(true);

		PlayerCharacter->ClientEntrancedOff();
		PlayerCharacter->Die();

		return true;
	}

	return false;
}

void UHuntGrabAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);

	Cast<AGoapAgentBase>(InCharacter)->ServerSetGrab(false);
	PlayerController->ClientStopSound2D();
	PlayerCharacter->ServerResetClickAmount(false, 0);

	InState.SetState("IsCloseToPlayer", false);
	InState.SetState("GrabStarted", false);
	InState.SetState("PlayerKill", false);
	InState.SetState("HasTarget", false);
	InState.SetState("PlayerID", -1);

	PlayerController = nullptr;
	PlayerCharacter = nullptr;
}

void UHuntGrabAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	Cast<AGoapAgentBase>(InCharacter)->ServerSetGrab(false);

	if (IsValid(PlayerController))
	{
		PlayerController->Client_UpdateTarget(FVector::ZeroVector, 0, 0, false);
		PlayerCharacter->ClientEntrancedOff();
		PlayerController->ClientStopSound2D();
	}
}
