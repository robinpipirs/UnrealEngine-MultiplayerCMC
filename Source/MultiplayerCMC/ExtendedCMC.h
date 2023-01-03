// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerCMCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ExtendedCMC.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_MAX			UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERCMC_API UExtendedCMC : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Extended : public FSavedMove_Character
	{
		uint8 Saved_bWantsToSprint:1;
		uint8 Saved_bPrevWantsToCrouch:1;
		
	public:
		enum CompressedFlags
		{
			FLAG_Sprint			= 0x10,
			FLAG_Dash			= 0x20,
			FLAG_Custom_2		= 0x40,
			FLAG_Custom_3		= 0x80,
		};
		
		FSavedMove_Extended();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Extended : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Extended(const UCharacterMovementComponent& ClientMovement);
		typedef FNetworkPredictionData_Client_Character Super;
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 350;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 500;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3;

	UPROPERTY(Transient) AMultiplayerCMCCharacter* ExtendedCharacterOwner;
	
	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

public:
	UExtendedCMC();

protected:
	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();
	UFUNCTION(BlueprintCallable) void CrouchPressed();

	UFUNCTION(BlueprintCallable) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintCallable) bool IsMovementMode(EMovementMode InMovementMode) const;
};
