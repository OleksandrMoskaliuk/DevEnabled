// Fill out your copyright notice in the Description page of Project Settings.

#include "SmoothCameraActorComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USmoothCameraActorComponent::USmoothCameraActorComponent() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = true;
  SmoothCameraMoveTimeline = new FTimeline();
}


USmoothCameraActorComponent::~USmoothCameraActorComponent() {
  if (SmoothCameraMoveTimeline) {
    delete SmoothCameraMoveTimeline;
    SmoothCameraMoveTimeline = nullptr;
  }
}

// Called when the game starts
void USmoothCameraActorComponent::BeginPlay() {
  UActorComponent::BeginPlay();

  if (MoveCurve) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,
                                     "BeginPlay!!!!!!!!!!!!!!!!!!!!!!!!!");

    FOnTimelineFloat TimelineCallback;
    FOnTimelineEventStatic TimelineFinishCallback;

    TimelineCallback.BindUFunction(this, TEXT("ChangeCameraDistanceSmoothly"));

    TimelineFinishCallback.BindUFunction(
        this, TEXT("ChangeCameraDistanceSmoothlyEnd"));

    SmoothCameraMoveTimeline->AddInterpFloat(MoveCurve, TimelineCallback);
    SmoothCameraMoveTimeline->SetTimelineFinishedFunc(TimelineFinishCallback);
  }
}

// Called every frame
void USmoothCameraActorComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
  if (bIsSmoothCameraTrigger) {
    SmoothCameraMoveTimeline->TickTimeline(DeltaTime);
  }
}

void USmoothCameraActorComponent::Move(float value) {
  float DistanceValue = value;
  if (SmoothCameraMoveTimeline->IsPlaying()) return;
  if (MoveCurve && SpringArm) {
    DistanceValue > 0 ? bIsSmoothCameraReversed = true
                      : bIsSmoothCameraReversed = false;

    SmoothCameraMoveTimeline->SetPlaybackPosition(0, true);
    SmoothCameraMoveTimeline->Play();
    bIsSmoothCameraTrigger = true;

  } else {
    // Clamp distance
    float MaxClampDistance = 1200.f;
    float MinClampDistance = 200.f;
    if (DistanceValue < 0 && SpringArm->TargetArmLength < MinClampDistance) {
      return;
    }
    if (DistanceValue > 0 && SpringArm->TargetArmLength > MaxClampDistance) {
      return;
    }
    SpringArm->TargetArmLength += DistanceValue * 25;
  }
}

void USmoothCameraActorComponent::Initialize(
    USpringArmComponent* SpringArmComp) {



  this->SpringArm = SpringArmComp;
}

void USmoothCameraActorComponent::ChangeCameraDistanceSmoothly() {
  ACharacter* Parent = Cast<ACharacter>(GetOwner());
  
  if (SpringArm) {
  
  float PlaybackPosition = SmoothCameraMoveTimeline->GetPlaybackPosition();
  float CurveValue = MoveCurve->GetFloatValue(PlaybackPosition);

  if (bIsSmoothCameraReversed) {
    CurveValue = -CurveValue;
  }

  const float AdjustedValue = CurveValue - SmoothCameraPreviousTimelineValue;
  GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,
                                   FString::SanitizeFloat(AdjustedValue));

  SpringArm->TargetArmLength += CurveValue;

  SmoothCameraPreviousTimelineValue = CurveValue;
  }
}

void USmoothCameraActorComponent::ChangeCameraDistanceSmoothlyEnd() {
  bIsSmoothCameraTrigger = false;
}
