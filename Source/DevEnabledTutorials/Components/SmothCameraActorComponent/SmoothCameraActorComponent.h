// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SmoothCameraActorComponent.generated.h"

class USpringArmComponent;

/* This class will make posiible to change camera distance to player more
 smoother way through curve */
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

  // Timeline begin
  UFUNCTION()
  void ChangeCameraDistanceSmoothlyBegin();

  // Timeline End
  UFUNCTION()
  void ChangeCameraDistanceSmoothlyEnd();

  void IncreaseCameraDistance();
  void DecreaseCameraDistance();

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;
  
  // Main function to change camera distance to player 
  void ChangeCameraDistance(float value);

 private:
  /* For change camera distance smoothly */
  struct FTimeline* SmoothCameraMoveTimeline;
  /* Decide in what direction move camera */
  bool bIsSmoothCameraReversed;
  /* Pointer to handle ArmLength distance */
  USpringArmComponent* SpringArm;
  int play_counter;
};
