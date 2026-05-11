// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/GetClosestPlayerAction.h"
#include "Core/LiminalGameMode.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Core/LiminalPlayerController.h"


void UGetClosestPlayerAction::Start(ACharacter* InCharacter, FWorldState& InState)
{

}

void UGetClosestPlayerAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	DelayTimer -= InDeltaTime;

	DebugDrawStringSimple(InCharacter->GetActorLocation(), FString::Printf(TEXT("Searching for Player: %.2f"), DelayTimer), FColor::Green, 1);

	if (DelayTimer <= 0)
	{
		int32 PlayerId = GetClosestPlayer(InCharacter);

		if (PlayerId != -1)
		{
			_Effects.SetState("PlayerID", PlayerId);
			bPlayerFound = true;
		}
		DelayTimer = DelayAmount;
	}
}

bool UGetClosestPlayerAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	return bPlayerFound;
}

void UGetClosestPlayerAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);

	bPlayerFound = false;
}

void UGetClosestPlayerAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	bPlayerFound = false;
}

int32 UGetClosestPlayerAction::GetClosestPlayer(ACharacter* InCharacter) const
{
	int32 PlayerCount = InCharacter->GetWorld()->GetAuthGameMode()->GetNumPlayers();
	float ClosestPlayerLocation = FLT_MAX;
	int32 TargetPlayer = -1;

	for (int i = 0; i < PlayerCount; i++)
	{
		ALiminalPlayerController* Controller = Cast<ALiminalPlayerController>(UGameplayStatics::GetPlayerController(InCharacter->GetWorld(), i));

		if (!IsValid(Controller)) continue;
		if (Controller->GetPlayerIsDead()) continue;

		FVector PlayerLocation = Controller->GetPawn()->GetActorLocation();
		FVector AgentLocation = InCharacter->GetActorLocation();
		float DistanceToPlayer = FVector::Dist(PlayerLocation, AgentLocation);
		if (ClosestPlayerLocation > DistanceToPlayer)
		{
			ClosestPlayerLocation = DistanceToPlayer;
			TargetPlayer = i;
		}
	}

	return TargetPlayer;
}