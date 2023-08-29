// Fill out your copyright notice in the Description page of Project Settings.

#include "DEV_SpringArmComponent.h"

UDEV_SpringArmComponent::UDEV_SpringArmComponent()
    : TargetArmLengthStartDistance(0.f),
      NewTargetArmLengthDistance(0.f),
      LerpTargetArmLengthTimerAlpha(0.f) {
  PrimaryComponentTick.bCanEverTick = false;
  TimerManager = nullptr;
}

void UDEV_SpringArmComponent::BeginPlay() {
  Super::BeginPlay();
  TimerManager = &(GetWorld()->GetTimerManager());
  // Only work through delegate
  InterpolateTargetArmLengthDelegate.BindUFunction(
      this, "InterpolateTargetArmLengthHandler");
  TargetArmLength = 1000;
  TargetArmLengthStartDistance = TargetArmLength;
}


void UDEV_SpringArmComponent::InterpolateTargetArmLength(float From, float To) {
  
  if (TimerManager &&
      TimerManager->IsTimerActive(InterpolateTargetArmLengthTimer)) {
    LerpTargetArmLengthTimerAlpha = 0.f;
    TimerManager->ClearTimer(InterpolateTargetArmLengthTimer);
    PrimaryComponentTick.bCanEverTick = false;
  }

  if (TimerManager) {
    TargetArmLengthStartDistance = From;
    NewTargetArmLengthDistance = To;
    // Do not work 
    /*TimerManager->SetTimer(
        InterpolateTargetArmLengthTimer, this,
        &UDEV_SpringArmComponent::InterpolateTargetArmLengthHandler, 60.f,
        true);*/
    TimerManager->SetTimer(InterpolateTargetArmLengthTimer,
                           InterpolateTargetArmLengthDelegate, 0.016f, true);
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, "UDEV_SpringArmComponent::InterpolateTargetArmLength");
  }
}

void UDEV_SpringArmComponent::InterpolateTargetArmLengthHandler() {
  
  if (LerpTargetArmLengthTimerAlpha >= 1.f) {
    LerpTargetArmLengthTimerAlpha = 0.f;
    TimerManager ? TimerManager->ClearTimer(InterpolateTargetArmLengthTimer)
                 : true;
    PrimaryComponentTick.bCanEverTick = false;
  } else {
    // If set to false TargetArmLength can't be changed in runtime
    PrimaryComponentTick.bCanEverTick = true;
    TargetArmLength =
        FMath::Lerp(TargetArmLengthStartDistance, NewTargetArmLengthDistance,
                    LerpTargetArmLengthTimerAlpha);
    LerpTargetArmLengthTimerAlpha += 0.1;
  }
  GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red,
                                   FString::SanitizeFloat(TargetArmLength));
}
