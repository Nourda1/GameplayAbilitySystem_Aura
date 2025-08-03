// Copyright Nourda


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//Allows Server replication to the clients
	bReplicates = true; 
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return; //if no blocking hit, exits function

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	// simplified version
	if (ThisActor != LastActor)
	{
		if (LastActor != nullptr)
		{
			LastActor->UnHighlightActor();
		}
 
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	
	/*
	 * LineTrace from cursor. There are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *	- do nothing
	 * B. LastActor is null && ThisActor is valid
	 *	- Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 *	-	Unhighlight LastActor
	 * D. Both actors valid, but LastActor != ThisActor
	 *  - Unhighlight LastActor, Highlight ThisActor
	 * E. Both actors are valid, and are the same actor
	 *  - Do nothing
	 

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//Case B
			ThisActor->HighlightActor();
		}
		else
		{
			//Case A - both are null, do nothing 
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHighlightActor();
		}
		else // Both actors are valid
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//Case E - Do nothing
			}
		}
	}  */
}
	

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	//Cursor Settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent); //Casts to EnhancedInputComponent and checks if valid

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,this, &AAuraPlayerController::Move); //Binds MoveAction to Move Function
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); //gets X and Y axis values in Vector2D
	//const FRotator Rotation = GetControlRotation(); //shows forward Direction based on camera view, controller rotation
	//const FRotator YawRotation(0,Rotation.Yaw,0); //initializes yaw rotation

	//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //Forward Vector axis
	//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Right Vector Axis

	if (APawn* ControlledPawn = GetPawn<APawn>()) //gets player controlled pawn
	{
		ControlledPawn->AddMovementInput(FVector::ForwardVector, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(FVector::RightVector, InputAxisVector.X);
	}
}
