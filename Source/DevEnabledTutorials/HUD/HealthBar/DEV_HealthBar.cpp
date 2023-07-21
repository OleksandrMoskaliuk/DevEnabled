// Fill out your copyright notice in the Description page of Project Settings.


#include "DEV_HealthBar.h"
#include "Components/ProgressBar.h"

void UDEV_HealthBar::SetHealth(float CurrentHealth, float MaxHealth) 
{
  if (HealthBar) {
    float Percent = FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 100.f);
    HealthBar->SetPercent(Percent);
  }
}
