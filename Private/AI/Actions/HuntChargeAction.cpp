// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actions/HuntChargeAction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"


void UHuntChargeAction::Start(ACharacter* InCharacter, FWorldState& InState)
{
	// Implementation for starting the action, e.g., setting up any necessary variables or states
	Controller = Cast<AAIController>(InCharacter->GetController());

	TargetActor = UGameplayStatics::GetPlayerCharacter(InCharacter->GetWorld(), InState.GetInt("PlayerID"));

	InCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void UHuntChargeAction::Tick(ACharacter* InCharacter, FWorldState& InState, float InDeltaTime)
{
	// Implementation for ticking the action, e.g., checking if the character has reached the target position
	if (IsValid(TargetActor) && IsValid(Controller))
	{
		FVector Location = TargetActor->GetActorLocation();
		Controller->MoveToLocation(Location);


		if (InState.GetBool("IsSeenByPlayer") && (LookMeter <= 1.0f))
		{
			FirstLookGrace -= InDeltaTime;
			FirstLookGrace = FMath::Clamp(FirstLookGrace, 0.0f, FLT_MAX);
			Timer += InDeltaTime;

			if ((Timer >= LookSeconds) && (FirstLookGrace <= 0))
			{
				LookMeter += LookMeterIncreaseAmount / 100.0f;

				Timer = 0.0f;
			}
			float LookingSpeed = FMath::Lerp(0.0f, HuntSpeed, LookMeter);
			InCharacter->GetCharacterMovement()->MaxWalkSpeed = LookingSpeed;

			if(LookingSpeed > CurrentSpeed)
			{
				CurrentSpeed = LookingSpeed;
			}

			DebugDrawStringSimple(InCharacter->GetActorLocation(), FString("Current Speed Percentage: " + FString::SanitizeFloat(100 * LookMeter)), FColor::Yellow, 1);
		}
		else
		{
			CurrentSpeed = FMath::FInterpTo(CurrentSpeed, HuntSpeed, InDeltaTime, NotLookingSpeedIncrease);

			InCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

			Timer = 0.0f;
		}

	}
}

bool UHuntChargeAction::IsActionComplete(ACharacter* InCharacter, FWorldState& InState) const
{
	// Check if the character is close enough to the target location
	FVector CharacterLocation = InCharacter->GetActorLocation();
	FVector TargetLocation = TargetActor ? TargetActor->GetActorLocation() : FVector::ZeroVector;
	bool bIsClose = FVector::Dist(CharacterLocation, TargetLocation) < DistanceToTarget;

	// Check if the character has line of sight to the target
	bool bHasLineOfSight = Controller ? Controller->LineOfSightTo(TargetActor, FVector(0, 0, 0)) : false;

	if (bIsClose && bHasLineOfSight)
	{
		return true;
	}

	return false;
}

void UHuntChargeAction::End(ACharacter* InCharacter, FWorldState& InState)
{
	// Implementation for ending the action, e.g., cleaning up any variables or states
	UE_LOG(LogTemp, Log, TEXT("Applying effects"));
	ApplyEffects(InState);

	if (IsValid(Controller))
	{
		Controller->StopMovement();
	}
}

void UHuntChargeAction::CancelAction(ACharacter* InCharacter, FWorldState& InState)
{
	if (IsValid(Controller))
	{
		Controller->StopMovement();
	}
}