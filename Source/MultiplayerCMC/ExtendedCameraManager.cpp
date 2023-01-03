// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedCameraManager.h"

#include "ExtendedCMC.h"
#include "MultiplayerCMCCharacter.h"
#include "Components/CapsuleComponent.h"

AExtendedCameraManager::AExtendedCameraManager()
{
}

void AExtendedCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
	
	if (AMultiplayerCMCCharacter* MultiplayerCMCCharacter = Cast<AMultiplayerCMCCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UExtendedCMC* Mcc = MultiplayerCMCCharacter->GetExtendedCharacterMovement();
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			Mcc->GetCrouchedHalfHeight() - MultiplayerCMCCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (Mcc->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}
