// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedCMC.h"

#include "GameFramework/Character.h"

bool UExtendedCMC::FSavedMove_Extended::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
                                                       float MaxDelta) const
{
	FSavedMove_Extended* NewExtendedMove = static_cast<FSavedMove_Extended*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewExtendedMove->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UExtendedCMC::FSavedMove_Extended::Clear()
{
	FSavedMove_Character::Clear();
	Saved_bWantsToSprint = 0;
}

uint8 UExtendedCMC::FSavedMove_Extended::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}

void UExtendedCMC::FSavedMove_Extended::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UExtendedCMC* CharacterMovement = Cast<UExtendedCMC>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UExtendedCMC::FSavedMove_Extended::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UExtendedCMC* CharacterMovement = Cast<UExtendedCMC>(C->GetCharacterMovement());
	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UExtendedCMC::FNetworkPredictionData_Client_Extended::FNetworkPredictionData_Client_Extended(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{
}

FSavedMovePtr UExtendedCMC::FNetworkPredictionData_Client_Extended::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Extended());
}

FNetworkPredictionData_Client* UExtendedCMC::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UExtendedCMC* MutableThis = const_cast<UExtendedCMC*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Extended(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

void UExtendedCMC::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UExtendedCMC::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		} else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
		
	}
}

UExtendedCMC::UExtendedCMC()
{
	NavAgentProps.bCanCrouch = true;
}

void UExtendedCMC::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UExtendedCMC::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

void UExtendedCMC::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;
}
