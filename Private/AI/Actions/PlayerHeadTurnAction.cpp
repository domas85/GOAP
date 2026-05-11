// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/PlayerHeadTurnAction.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/GoapAgentBase.h"
#include "Camera/CameraComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/LiminalPlayerController.h"
#include "Core/LiminalPlayerCharacter.h"

void UPlayerHeadTurnAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	CurrentRotationTime = 0;
	PlayerController = Cast<ALiminalPlayerController>(UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), InState.GetInt("PlayerID"))->GetController());

	if (!IsValid(PlayerController)) return;
	PlayerLookRotation = PlayerController->GetControlRotation();
	PlayerController->ClientStartCameraShake(CameraShakeClass, 1.0f);
	Cast<ALiminalPlayerCharacter>(PlayerController->GetPawn())->ClientSetSpeedPercentage(0);
	Cast<ALiminalPlayerCharacter>(PlayerController->GetPawn())->ClientPlaySound2D(HeadTurnSFX);

	// Rotate Agent to Player 

	FVector AgentLocation = InCharacter->GetActorLocation();
	FVector PlayerLocation = Cast<APawn>(PlayerController->GetPawn())->GetActorLocation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(AgentLocation, PlayerLocation);
	InCharacter->SetActorRotation(DesiredRotation);
}

void UPlayerHeadTurnAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	if (!IsValid(PlayerController)) return;
	TObjectPtr<AAIController> AgentController = Cast<AAIController>(InCharacter->GetController());

	FVector AgentLocation = InCharacter->GetActorLocation();

	FVector PlayerLocation = Cast<APawn>(PlayerController->GetPawn())->FindComponentByClass<UCameraComponent>()->GetComponentLocation();

	DesiredLookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, AgentLocation + LookAtOffset);

	FRotator NewRotation = UKismetMathLibrary::RLerp(PlayerLookRotation, DesiredLookAtRotation, CurrentRotationTime, true);
	PlayerController->Client_ForceLookAt(NewRotation);

	CurrentRotationTime += CameraTurnSpeedMultiplier * InDeltaTime;
	float Temp = FMath::Clamp(CurrentRotationTime, 0.0f, 1.0f);
	CurrentRotationTime = Temp;
}

bool UPlayerHeadTurnAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	if (!IsValid(PlayerController)) return false;
	FRotator Delta = (PlayerController->GetControlRotation() - DesiredLookAtRotation).GetNormalized();
	if ((FMath::Abs(Delta.Yaw) < 1.0f) && (FMath::Abs(Delta.Pitch) < 1.0f))
	{
		return true;
	}

	return false;
}

void UPlayerHeadTurnAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	ApplyEffects(InState);

	int32 Stacks = InState.GetInt("GrabStacks");
	Stacks++;
	InState.SetState("GrabStacks", Stacks);


	Cast<ALiminalPlayerCharacter>(PlayerController->GetPawn())->ClientSetSpeedPercentage(100);

	if (IsValid(PlayerController))
	{
		PlayerController->ClientStopCameraShake(CameraShakeClass);
	}
}

void UPlayerHeadTurnAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	if (IsValid(PlayerController))
	{
		PlayerController->ClientStopCameraShake(CameraShakeClass);
	}
}

