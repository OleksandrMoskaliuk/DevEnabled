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

 UPROPERTY(EditAnywhere, meta = (BindWidget))
 class UProgressBar* HealthBar;

};
