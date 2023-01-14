// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SmoothCameraActorComponent.generated.h"


class USpringArmComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEVENABLEDTUTORIALS_API USmoothCameraActorComponent
    : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  USmoothCameraActorComponent();
  ~USmoothCameraActorComponent();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
            Category = "Smooth Camera Movement")
  class UCurveFloat* MoveCurve;

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

UFUNCTION()
  void ChangeCameraDistanceSmoothly();

  UFUNCTION()
  void ChangeCameraDistanceSmoothlyEnd();

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;

  void Move(float value);

  void Initialize(USpringArmComponent* SpringArmComp);
 private:
  /* For change camera distance smoothly */
  struct FTimeline* SmoothCameraMoveTimeline;
  bool bIsSmoothCameraTrigger;
  bool bIsSmoothCameraReversed;
  float SmoothCameraPreviousTimelineValue = 0.f;
  USpringArmComponent* SpringArm;

  

};
