// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/GetTargetPlayerAction.h"
#include "GameFramework/Character.h"
#include "Core/LiminalGameMode.h"
#include "Core/LiminalPlayerController.h"
#include "Kismet/GameplayStatics.h"


void UGetTargetPlayerAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	DelayTime = SearchDelay;
}

void UGetTargetPlayerAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{

	int32 PlayerCount = InCharacter->GetWorld()->GetAuthGameMode()->GetNumPlayers();
	int32 RandomPlayer = FMath::RandRange(0, PlayerCount - 1);
	ALiminalPlayerController* Controller = Cast<ALiminalPlayerController>(UGameplayStatics::GetPlayerController(InCharacter->GetWorld(), RandomPlayer));

	DelayTime -= InDeltaTime;

	if (DelayTime > 0) return;

	if (IsValid(Controller))
	{
		if (!Controller->GetPlayerIsDead())
		{
			TargetPlayer = RandomPlayer;
			_Effects.SetState("PlayerID", TargetPlayer);
			
			return;
		}
	}

	DelayTime = SearchDelay;
}

bool UGetTargetPlayerAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	if (TargetPlayer > -1)
	{
		return true;
	}
	return false;
}

void UGetTargetPlayerAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	// Implementation for ending the action, e.g., cleaning up any variables or states

	ApplyEffects(InState);
}

void UGetTargetPlayerAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{

}
