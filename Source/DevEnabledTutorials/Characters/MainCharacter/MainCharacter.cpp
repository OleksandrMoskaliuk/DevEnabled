// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
// Inputs
#include <GameFramework/PawnMovementComponent.h>

#include "../../Components/SmothCameraActorComponent/SmoothCameraActorComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
// Other
#include "../../Interfaces/DEV_Interact.cpp"

AMainCharacter::AMainCharacter() : FocusedActor(nullptr) {
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
      CreateDefaultSubobject<UCameraComponent>("Camera Component");
  CameraComponent->SetupAttachment(SpringArmComponent);

  //SmoothCameraComponent = CreateDefaultSubobject<USmoothCameraActorComponent>(
  //    "Smooth Camera Component");

  SphereOverlapComponent = CreateDefaultSubobject<USphereComponent>(
      "SphereOverlap");
  SphereOverlapComponent->SetupAttachment(GetRootComponent());

  BaseTurnRate = 45.f;
  BaseLookUpAtRate = 45.f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() { Super::BeginPlay();

  SphereOverlapComponent->OnComponentBeginOverlap.AddDynamic(
      this, &AMainCharacter::OverlapBegin);


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

void AMainCharacter::FireLineTrace(const FInputActionValue& Value) {
  FindAcotrByLineTrace();
}

AActor* AMainCharacter::FindAcotrByLineTrace(bool ShowLine) {
  FVector Start;
  FVector End;
  FRotator Rot;
  FHitResult OutHit;
  FCollisionQueryParams CollisionParams;
  AActor* Actor = nullptr;
  GetController()->GetPlayerViewPoint(Start, Rot);
  End = Start + (Rot.Vector() * 4000);
  if (ShowLine) {
  DrawDebugLine(GetWorld(), Start + (Rot.Vector() * 500), End, FColor::Green,
                false, 2.f);
  }
  if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility,
                                           CollisionParams)) {
    Actor = OutHit.GetActor();
    if (OutHit.bBlockingHit) {
      GEngine->AddOnScreenDebugMessage(
          18764, 1.f, FColor::Red,
          FString::Printf(TEXT("You are hitting: %s"),
                          *OutHit.GetActor()->GetName()));
      GEngine->AddOnScreenDebugMessage(
          18436, 1.f, FColor::Purple,
          FString::Printf(TEXT("Impact Point: %s"),
                          *OutHit.ImpactPoint.ToString())); 
      GEngine->AddOnScreenDebugMessage(
          19854, 1.f, FColor::Blue,
          FString::Printf(TEXT("Normal Point: %s"),
                          *OutHit.ImpactNormal.ToString()));
    }
  }
  return Actor;
}

void AMainCharacter::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
                                  AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult) {
  GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Hello from CPP!");
  if (OtherActor) {
    IDEV_Interact* AInterface = Cast<IDEV_Interact>(OtherActor);
    if (AInterface) {
      AInterface->Execute_OnInteract(OtherActor);
    }
  }

}

void AMainCharacter::PlayerInteract(const FInputActionValue& Value) {
  AActor* IActor = FindAcotrByLineTrace();
    if (IActor) {
      IDEV_Interact* AInterface = Cast<IDEV_Interact>(IActor);
      if (AInterface) {
        AInterface->Execute_OnInteract(IActor);
      }
    }
 }

void AMainCharacter::ChangeCameraDistance(const FInputActionValue& Value) {  
 if (Controller != nullptr) {
      CCD_Delegate.Broadcast(Value.Get<float>());
  }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) { 
 Super::Tick(DeltaTime);

  auto focus = [](AActor* Actor, bool focus_or_unfocus) {
    if (Actor) {
     IDEV_Interact *Interface = Cast<IDEV_Interact>(Actor);
      if (Interface) {
       if (focus_or_unfocus) {
       Interface->Execute_StartFocus(Actor);
       } else {
       Interface->Execute_EndFocus(Actor);
       }
      }
    }
  };

 if (FocusedActor) {
    AActor* CurrentActor = FindAcotrByLineTrace();
    if (FocusedActor != CurrentActor) {
      focus(FocusedActor, false);
    }
    FocusedActor = CurrentActor;
    focus(FocusedActor, true);   
 } else {
    FocusedActor = FindAcotrByLineTrace();
    focus(FocusedActor, true);
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
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
                                     "IA_ChangeCameraDistance");
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::ChangeCameraDistance);
  }

  CurrentInputAction = *InputActionsMap.Find("IA_FireLineTrace");
  if (CurrentInputAction) {
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, "IA_FireLineTrace");
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::FireLineTrace);
  }

  CurrentInputAction = nullptr;
  CurrentInputAction = *InputActionsMap.Find("IA_PlayerInteract");
  if (CurrentInputAction) {
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
                                     "IA_PlayerInteract");
    EnhancedInputComponent->BindAction(CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::PlayerInteract);
  }
}
