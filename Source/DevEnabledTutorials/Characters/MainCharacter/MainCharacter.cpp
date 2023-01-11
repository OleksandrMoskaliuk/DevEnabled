// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Controls
#include "../../Inputs/GameplayTagsSingleton/MyInputConfigData.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"

// Sets default values
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
void AMainCharacter::BeginPlay() { Super::BeginPlay(); }

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
    GEngine->AddOnScreenDebugMessage(
        345, 1, FColor::Green, "Y = " + FString::SanitizeFloat(LookValue.Y));
    GEngine->AddOnScreenDebugMessage(
        479, 1, FColor::Green, "X = " + FString::SanitizeFloat(LookValue.X));

    APlayerController* const PC = CastChecked<APlayerController>(Controller);
    PC->IsLookInputIgnored();
    GEngine->AddOnScreenDebugMessage(
        179, 1, FColor::Green,
        "PC->IsLookInputIgnored() = " +
            FString(PC->IsLookInputIgnored() ? "true" : "false"));

    AddControllerYawInput(LookValue.X * BaseTurnRate *
                          GetWorld()->GetDeltaSeconds() * (-1.f));

    AddControllerPitchInput(LookValue.Y * BaseLookUpAtRate *
                            GetWorld()->GetDeltaSeconds());
  }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

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
  if (InputMapping) {
    Subsystem->AddMappingContext(InputMapping, 0);
  }

  // Get the EnhancedInputComponent
  UEnhancedInputComponent* PEI =
      Cast<UEnhancedInputComponent>(PlayerInputComponent);
  // Bind the actions
  if (InputActions) {
    PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this,
                    &AMainCharacter::Move);
    PEI->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this,
                    &AMainCharacter::Look);
  }
}
