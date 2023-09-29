// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "DEV_SpringArmComponent.generated.h"

/**
 *
 */
UCLASS()
class DEVENABLEDTUTORIALS_API UDEV_SpringArmComponent
    : public USpringArmComponent {
  GENERATED_BODY()
 public:
  UDEV_SpringArmComponent();
  void BeginPlay() override;
  void InterpolateTargetArmLength(float From, float To);
  UFUNCTION()
  void InterpolateTargetArmLengthHandler();
  void TickComponent(float DeltaTime, enum ELevelTick TickType,
                     FActorComponentTickFunction* ThisTickFunction) override;
 private:
  float TargetArmLengthStartDistance;
  float NewTargetArmLengthDistance;
  // from 0.0 to 1;
  FTimerManager* TimerManager;
  float LerpTargetArmLengthTimerAlpha;
  FTimerHandle InterpolateTargetArmLengthTimer;
  FTimerDelegate InterpolateTargetArmLengthDelegate;
 public:
};
