// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/DisengageTeleportAction.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AI/GoapAgentBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Core/LiminalGameMode.h"
#include "Core/LiminalPlayerCharacter.h"

void UDisengageTeleportAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	bTeleportSuccess = false;

	int32 PlayerCount = InCharacter->GetWorld()->GetAuthGameMode()->GetNumPlayers();

	for (int i = 0; i < PlayerCount; i++)
	{

		TObjectPtr<ALiminalPlayerCharacter> PlayerChar = Cast<ALiminalPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), i));

		if (!IsValid(PlayerChar)) continue;

		if (PlayerChar->GetIsLookingAtThreat())
		{
			PlayerChar->OnThreatTeleport.Broadcast();
		}

	}
}

void UDisengageTeleportAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	FVector PathStart = Cast<APawn>(InCharacter)->GetActorLocation() - FVector(0, 0, 70);
	// get random point in navmesh for teleport location
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(InCharacter->GetWorld());

	NavSystem->GetRandomPointInNavigableRadius(PathStart, TeleportRadius, RandomTeleportPoint);

	NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(InCharacter->GetWorld(), PathStart, RandomTeleportPoint);

	float Distance = FMath::Abs(RandomTeleportPoint.Location.Length() - PathStart.Length());

	if (!NavPath || Distance < MinimumTeleportDistance || Distance > (TeleportRadius * 2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Nav Path Null or Teleport distance is too short/far - can not teleport"));
		return;
	}

	if (!NavPath->IsPartial())
	{
		Cast<AGoapAgentBase>(InCharacter)->Multicast_PlaySoundAtAgentLocation(TeleportSFX);

		Cast<AGoapAgentBase>(InCharacter)->TeleportTo(RandomTeleportPoint.Location + TeleportLocationOffset, InCharacter->GetActorRotation());
		DrawDebugSphere(InCharacter->GetWorld(), RandomTeleportPoint.Location, 100, 10, FColor::Blue, false, 5.0f);
		DrawDebugSphere(InCharacter->GetWorld(), RandomTeleportPoint.Location, 20, 5, FColor::Blue, false, 5.0f);
		bTeleportSuccess = true;
	}
}

bool UDisengageTeleportAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	return bTeleportSuccess;
}

void UDisengageTeleportAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	RotateToClosestPlayer(InCharacter);

	ApplyEffects(InState);

	// not sure if this should be here, probably better to make a separate action for resetting stuff
	InState.SetState("DisengageDone", false);
	InState.SetState("DisengageTeleport", false);
	InState.SetState("IsCloseToPlayer", false);
	InState.SetState("GrabStarted", false);
	InState.SetState("GrabCompleted", false);
	InState.SetState("HasTarget", false);
	InState.SetState("PlayerID", -1);
}

void UDisengageTeleportAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{

}


void UDisengageTeleportAction::RotateToClosestPlayer(ACharacter* InCharacter)
{
	int32 PlayerCount = InCharacter->GetWorld()->GetAuthGameMode()->GetNumPlayers();
	float ClosestPlayerDistance = FLT_MAX;
	FVector ClosestPlayerLocation;

	for (int i = 0; i < PlayerCount; i++)
	{
		FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), i)->GetActorLocation();
		FVector AgentLocation = InCharacter->GetActorLocation();
		float DistanceToPlayer = FVector::Dist(PlayerLocation, AgentLocation);
		if (ClosestPlayerDistance > DistanceToPlayer)
		{
			ClosestPlayerDistance = DistanceToPlayer;
			ClosestPlayerLocation = PlayerLocation;
		}
	}

	// Rotate Agent to Player 

	FVector AgentLocation = InCharacter->GetActorLocation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(AgentLocation, ClosestPlayerLocation);
	InCharacter->SetActorRotation(DesiredRotation);

}