// Fill out your copyright notice in the Description page of Project Settings.

#include "DEV_HealthBar.h"

#include "Components/ProgressBar.h"

void UDEV_HealthBar::SetHealth(float CurrentHealth, float MaxHealth) {
  bHealthPercentTrigger = true;
  if (HealthBar) {
    HealthBarPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 100.f);
  }

}

void UDEV_HealthBar::NativeTick(const FGeometry& MyGeometry, float DeltaTime) {
  Super::NativeTick(MyGeometry, DeltaTime);

 


  GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
                                   FString::SanitizeFloat(HealtLerpAlpha));
}

void UDEV_HealthBar::SmoothHealthBarHandler(float DeltaTime) {
  if (bHealthPercentTrigger) {
    HealtLerpAlpha += DeltaTime;
    HealthBar->SetPercent(
        FMath::Lerp(PrevHealthBarPercent, HealthBarPercent, HealtLerpAlpha));
    if (HealtLerpAlpha >= 1) {
      HealtLerpAlpha = 0;
      bHealthPercentTrigger = false;
      PrevHealthBarPercent = HealthBarPercent;
    }
  }
}
