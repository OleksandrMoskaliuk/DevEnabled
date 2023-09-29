// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
// Inputs
#include <GameFramework/PawnMovementComponent.h>

#include "../../Components/DEV_CameraComponent/DEV_CameraComponent.h"
#include "../../Components/DEV_SpringArmComponent/DEV_SpringArmComponent.h"
#include "DevEnabledTutorials\Components\DEV_CameraComponent\DEV_CameraComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
// Other
#include "../../Interfaces/DEV_Interact.cpp"

AMainCharacter::AMainCharacter() : FocusedActor(nullptr) {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  GetRootComponent()->SetComponentTickEnabled(false);
  StaticMeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>("Player mesh");
  StaticMeshComponent->SetupAttachment(GetRootComponent());

  DEV_SpringArmComponent =
      CreateDefaultSubobject<UDEV_SpringArmComponent>("Spring Arm componnet");
  DEV_SpringArmComponent->SetupAttachment(GetRootComponent());
  // SpringArmComponent->TargetArmLength = 400.f;

  DEV_CameraComponent =
      CreateDefaultSubobject<UDEV_CameraComponent>("DEV Camera Component");
  DEV_CameraComponent->SetupAttachment(DEV_SpringArmComponent);

  SphereOverlapComponent =
      CreateDefaultSubobject<USphereComponent>("SphereOverlap");
  SphereOverlapComponent->SetupAttachment(GetRootComponent());
  InputMappingContext = nullptr;
  BaseTurnRate = 45.f;
  BaseLookUpAtRate = 45.f;
  SquashGapOffsetOnLanding = 0;
  SquashStrength = 1;
}

//AMainCharacter::~AMainCharacter() 
//{
// Super::~ACharacter(); 
// if (SquashTimeline) {
// delete SquashTimeline;
// SquashTimeline = nullptr;
// }
//}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
  Super::BeginPlay();

  if (SquashCurve && StaticMeshComponent) {
    StartMeshScale = StaticMeshComponent->GetComponentScale();
    SquashOffsetRelativeMeshLocation =
        StaticMeshComponent->GetRelativeLocation();
    FOnTimelineFloat SquashTimelineCallback;
    SquashTimelineCallback.BindUFunction(this, "SquashTimelineProgress");
    SquashTimeline.AddInterpFloat(SquashCurve, SquashTimelineCallback);
    SquashTimeline.SetLooping(false);
  }

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
    GEngine->AddOnScreenDebugMessage(
        345, 1, FColor::Green, "Y = " + FString::SanitizeFloat(LookValue.Y));
    GEngine->AddOnScreenDebugMessage(
        479, 1, FColor::Green, "X = " + FString::SanitizeFloat(LookValue.X));

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
  if (!SquashTimeline.IsPlaying())
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
  if (OtherActor) {
    IDEV_Interact* AInterface = Cast<IDEV_Interact>(OtherActor);
    if (AInterface) {
      AInterface->Execute_OnInteract(OtherActor);
      AInterface->Execute_OnInteractWithActor(OtherActor, this);
    }
  }
}

void AMainCharacter::SquashTimelineProgress(float value) 
{
  // When player land on ground squash cause gap between ground and mesh.
  FVector NewScale = FMath::Lerp(
      StartMeshScale,
      FVector(StartMeshScale.X, StartMeshScale.Y, StartMeshScale.Z * SquashStrength), value);
  StaticMeshComponent->SetRelativeScale3D(NewScale);
  
  // Fixing gap between mesh and ground
  FVector NewLocation = FMath::Lerp(
      SquashOffsetRelativeMeshLocation,
                  FVector(SquashOffsetRelativeMeshLocation.X,
                          SquashOffsetRelativeMeshLocation.Y,
              SquashOffsetRelativeMeshLocation.Z + SquashGapOffsetOnLanding),
      value);
  StaticMeshComponent->SetRelativeLocation(NewLocation);




  /* FVector LocationOffset = FVector(SquashStartLocation.X,
     SquashStartLocation.Y, SquashStartLocation.Z * NewScale.Z);

 FVector LocOffset = FVector(LocMesh.X, LocMesh.Y, LocMesh.Z);
 FVector LocMesh = StaticMeshComponent->GetRelativeLocation();

  FString locX = FString::SanitizeFloat(LocMesh.X);
  FString locY = FString::SanitizeFloat(LocMesh.X);
  FString locZ = FString::SanitizeFloat(LocMesh.X);
  GEngine->AddOnScreenDebugMessage(381, 0.1f, FColor::Green,
                                   FString("X=" + locX + "Y=" + locY + "Z=" + locZ));*/
}

void AMainCharacter::PlayerInteract(const FInputActionValue& Value) {
  AActor* IActor = FindAcotrByLineTrace();
  if (IActor) {
    IDEV_Interact* AInterface = Cast<IDEV_Interact>(IActor);
    if (AInterface) {
      AInterface->Execute_OnInteract(IActor);
      FVector Start;
      FRotator Rot;
      GetController()->GetPlayerViewPoint(Start, Rot);
      // AInterface->Execute_OnImpulse(IActor,
      // DEV_CameraComponent->GetForwardVector() * 200);
      AInterface->Execute_OnImpulse(IActor, Rot.Vector() * 200);
    }
  }
}

void AMainCharacter::SphereImpulse(const FInputActionValue& Value) {
  FVector Start;
  FVector End;
  FRotator Rot;
  FHitResult OutHit;
  FCollisionQueryParams CollisionParams;
  AActor* Actor = nullptr;
  GetController()->GetPlayerViewPoint(Start, Rot);
  End = Start + (Rot.Vector() * 4000);

  if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility,
                                           CollisionParams)) {
    if (OutHit.bBlockingHit) {
      TArray<FHitResult> HitArray;
      float SphereRadius = 500;
      FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);

      GetWorld()->SweepMultiByChannel(HitArray, OutHit.Location,
          OutHit.ImpactPoint + FVector(1.f, 1.f, 1.f),
                                      FQuat::Identity, ECC_WorldStatic, Sphere);

        DrawDebugSphere(GetWorld(), OutHit.Location, SphereRadius, 20,
                        FColor::Yellow, false, 1.f);

        for (auto SingleHit : HitArray) {
          if (UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(
                  SingleHit.GetActor()->GetRootComponent())) {

            HitMesh->AddRadialImpulse(OutHit.ImpactPoint, SphereRadius,
                                      500,
                                      ERadialImpulseFalloff::RIF_Constant,
                                      true);

           /*HitMesh->AddImpulse(OutHit.ImpactPoint.ForwardVector * 2000 );*/
          }
        }
      
    }
  }
}

void AMainCharacter::ChangeCameraDistance(const FInputActionValue& Value) {
  float From = DEV_SpringArmComponent->TargetArmLength;
  float To =
      DEV_SpringArmComponent->TargetArmLength + (60 * Value.Get<float>());
  float MaxCameraDistanceToCharacter = 1000;
  float MinCameraDistanceToCharacter = 100;
  // Clamp distance to player
  To >= MaxCameraDistanceToCharacter || To <= MinCameraDistanceToCharacter
      ? false
      : DEV_SpringArmComponent->InterpolateTargetArmLength(From, To);
  // this->UDEVCameraComponent->ChangeCameraDistance(1);
  /*if (Controller != nullptr) {
       CCD_Delegate.Broadcast(Value.Get<float>());
   }*/
  CCD_Delegate.Broadcast(Value.Get<float>());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  auto focus = [](AActor* Actor, bool focus_or_unfocus) {
    if (Actor) {
      IDEV_Interact* Interface = Cast<IDEV_Interact>(Actor);
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

  SquashTimeline.TickTimeline(DeltaTime);
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

  if (auto CurrentInputAction = InputActionsMap.Find("IA_Move")) {
    EnhancedInputComponent->BindAction(*CurrentInputAction,
                                       ETriggerEvent::Triggered, this,
                                       &AMainCharacter::Move);
  }

  if (auto InputAction = InputActionsMap.Find("IA_Look")) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ERROR 254");
    EnhancedInputComponent->BindAction(*InputAction, ETriggerEvent::Triggered,
                                       this, &AMainCharacter::Look);
  }

  if (auto InputAction = InputActionsMap.Find("IA_Jump")) {
    EnhancedInputComponent->BindAction(*InputAction, ETriggerEvent::Triggered,
                                       this, &AMainCharacter::Jump);
  }

  if (auto InputAction = InputActionsMap.Find("IA_ChangeCameraDistance")) {
    EnhancedInputComponent->BindAction(*InputAction, ETriggerEvent::Triggered,
                                       this,
                                       &AMainCharacter::ChangeCameraDistance);
  }

  if (auto InputAction = InputActionsMap.Find("IA_FireLineTrace")) {
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, "IA_FireLineTrace");
    EnhancedInputComponent->BindAction(*InputAction, ETriggerEvent::Triggered,
                                       this, &AMainCharacter::FireLineTrace);
  }

  if (auto InputAction = InputActionsMap.Find("IA_PlayerInteract")) {
    /* GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
                                      "IA_PlayerInteract");*/
    EnhancedInputComponent->BindAction(*InputAction, ETriggerEvent::Triggered,
                                       this, &AMainCharacter::PlayerInteract);
  }

  if (auto InputAction = InputActionsMap.Find("IA_SprhereImpulse")) {
    EnhancedInputComponent->BindAction(*InputAction, ETriggerEvent::Triggered,
                                       this, &AMainCharacter::SphereImpulse);
  }
}

void AMainCharacter::Landed(const FHitResult& Hit) 
{ 
 GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString("OnLanded!!!"));
 Super::Landed(Hit);
 SquashTimeline.PlayFromStart();

}
