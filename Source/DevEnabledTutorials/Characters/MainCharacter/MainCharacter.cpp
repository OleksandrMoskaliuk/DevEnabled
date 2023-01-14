// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Inputs
#include <GameFramework/PawnMovementComponent.h>

#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"

AMainCharacter::AMainCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  StaticMeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>("Player mesh");
  StaticMeshComponent->SetupAttachment(GetRootComponent());

  SpringArmComponent =
      CreateDefaultSubobject<USpringArmComponent>("Spring Arm componnet");
  SpringArmComponent->SetupAttachment(GetRootComponent());
  // SpringArmComponent->TargetArmLength = 400.f;

  CameraComponent =
      CreateDefaultSubobject<UCameraComponent>("Camera componnet");
  CameraComponent->SetupAttachment(SpringArmComponent);

 
  BaseTurnRate = 45.f;
  BaseLookUpAtRate = 45.f;
}



// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
  Super::BeginPlay();
  if (MoveCurve) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, "BeginPlay!!!!!!!!!!!!!!!!!!!!!!!!!");

    FOnTimelineFloat TimelineCallback;
    FOnTimelineEventStatic TimelineFinishCallback;

    TimelineCallback.BindUFunction(this,TEXT("ChangeCameraDistanceSmoothly"));

    TimelineFinishCallback.BindUFunction(this, TEXT("ChangeCameraDistanceSmoothlyEnd"));

    SmoothCameraMoveTimeline.AddInterpFloat(MoveCurve, TimelineCallback);
    SmoothCameraMoveTimeline.SetTimelineFinishedFunc(TimelineFinishCallback);

  }
}

void AMainCharacter::MoveForward() {
  GEngine->AddOnScreenDebugMessage(123, 2, FColor::Red, "Move is here");
}

void AMainCharacter::Move(const FInputActionValue& Value) {
  if (Controller != nullptr) {
    const FVector2D MoveValue = Value.Get<FVector2D>();
    const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

    // Forward/Backward direction
    if (MoveValue.Y != 0.f) {
      // Get forward vector
      const FVector Direction =
          MovementRotation.RotateVector(FVector::ForwardVector);

      AddMovementInput(Direction, MoveValue.Y);
    }

    // Right/Left direction
    if (MoveValue.X != 0.f) {
      // Get right vector
      const FVector Direction =
          MovementRotation.RotateVector(FVector::RightVector);

      AddMovementInput(Direction, MoveValue.X);
    }
  }
}

void AMainCharacter::Look(const FInputActionValue& Value) {
  if (Controller != nullptr) {
    FVector2D LookValue = Value.Get<FVector2D>();
    /*GEngine->AddOnScreenDebugMessage(
        345, 1, FColor::Green, "Y = " + FString::SanitizeFloat(LookValue.Y));
    GEngine->AddOnScreenDebugMessage(
        479, 1, FColor::Green, "X = " + FString::SanitizeFloat(LookValue.X));*/

    /*APlayerController* const PC = CastChecked<APlayerController>(Controller);
    GEngine->AddOnScreenDebugMessage(
        179, 1, FColor::Green,
        "PC->IsLookInputIgnored() = " +
            FString(PC->IsLookInputIgnored() ? "true" : "false"));*/

    AddControllerYawInput(LookValue.X * BaseTurnRate *
                          GetWorld()->GetDeltaSeconds() * (-1.f));

    AddControllerPitchInput(LookValue.Y * BaseLookUpAtRate *
                            GetWorld()->GetDeltaSeconds());
  }
}

void AMainCharacter::Jump(const FInputActionValue& Value) {
  if (Controller != nullptr && !GetMovementComponent()->IsFalling()) {
    ACharacter::Jump();
  }
}

void AMainCharacter::ChangeCameraDistance(const FInputActionValue& Value) {
  if (Controller != nullptr) {
    float DistanceValue = Value.Get<float>();
    if (MoveCurve) {
      DistanceValue > 0 ? bIsSmoothCameraReversed = true
                        : bIsSmoothCameraReversed = false; 

      SmoothCameraMoveTimeline.SetPlaybackPosition(0,true);
      SmoothCameraMoveTimeline.Play();
      bIsSmoothCameraTrigger = true;

    } else {
      // Clamp distance
      float MaxClampDistance = 1200.f;
      float MinClampDistance = 200.f;
      GEngine->AddOnScreenDebugMessage(
          -1, 2.f, FColor::Orange,
          FString::SanitizeFloat(SpringArmComponent->TargetArmLength));
      if (DistanceValue < 0 &&
          SpringArmComponent->TargetArmLength < MinClampDistance) {
        return;
      }
      if (DistanceValue > 0 &&
          SpringArmComponent->TargetArmLength > MaxClampDistance) {
        return;
      }
      SpringArmComponent->TargetArmLength += DistanceValue * 25;
    }
  }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) { 
  Super::Tick(DeltaTime);
  if (bIsSmoothCameraTrigger) {
    SmoothCameraMoveTimeline.TickTimeline(DeltaTime);
  }
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  // Get the player controller
  APlayerController* PC = Cast<APlayerController>(GetController());
  // Get the local player subsystem
  UEnhancedInputLocalPlayerSubsystem* Subsystem =
      ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
          PC->GetLocalPlayer());
  // Clear out existing mapping, and add our mapping
  Subsystem->ClearAllMappings();

  if (InputMappingContext) {
    Subsystem->AddMappingContext(InputMappingContext, 0);
  }

  // Get the EnhancedInputComponent
  UEnhancedInputComponent* EnhancedInputComponent =
      Cast<UEnhancedInputComponent>(PlayerInputComponent);
  // Bind the actions

  UInputAction* CurrentInputAction = nullptr;
  CurrentInputAction = *InputActionsMap.Find("IA_Move");
  if (CurrentInputAction) {
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::Move);
  }

  CurrentInputAction = *InputActionsMap.Find("IA_Look");
  if (CurrentInputAction) {
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::Look);
  }

  CurrentInputAction = *InputActionsMap.Find("IA_Jump");
  if (CurrentInputAction) {
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::Jump);
  }

  CurrentInputAction = *InputActionsMap.Find("IA_ChangeCameraDistance");
  if (CurrentInputAction) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,
                                     "IA_ChangeCameraDistance");
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::ChangeCameraDistance);
  }
}

void AMainCharacter::ChangeCameraDistanceSmoothly() {

  float PlaybackPosition = SmoothCameraMoveTimeline.GetPlaybackPosition();
  float CurveValue = MoveCurve->GetFloatValue(PlaybackPosition);

  if (bIsSmoothCameraReversed) {
    CurveValue = -CurveValue;
  }

  const float AdjustedValue = CurveValue - SmoothCameraPreviousTimelineValue;
  GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,
                                   FString::SanitizeFloat(AdjustedValue));

 SpringArmComponent->TargetArmLength += CurveValue;

  SmoothCameraPreviousTimelineValue = CurveValue;
}

void AMainCharacter::ChangeCameraDistanceSmoothlyEnd() {
  bIsSmoothCameraTrigger = false;
}

