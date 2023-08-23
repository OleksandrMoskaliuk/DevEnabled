// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DEV_HealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DEVENABLEDTUTORIALS_API UDEV_HealthBar : public UUserWidget
{
	GENERATED_BODY()
 
 public:

 void SetHealth(float CurrentHealth, float MaxHealth);

 void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

 UPROPERTY(EditAnywhere, meta = (BindWidget))
 class UProgressBar* HealthBar;

 private:
 bool bHealthPercentTrigger = false;
 float HealthBarPercent = 0;
 float PrevHealthBarPercent = 0;
 float HealtLerpAlpha = 0;
 void SmoothHealthBarHandler(float DeltaTime);
};
