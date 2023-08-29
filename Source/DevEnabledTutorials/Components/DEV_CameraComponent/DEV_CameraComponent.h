// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "DEV_CameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEVENABLEDTUTORIALS_API UDEV_CameraComponent : public UCameraComponent
{
  UDEV_CameraComponent();
	GENERATED_BODY()
 public:
 void BeginPlay() override;
 void ChangeCameraDistance(float Direction);
 private:
 class USpringArmComponent* SpringArm;


};
