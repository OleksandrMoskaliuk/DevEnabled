// Fill out your copyright notice in the Description page of Project Settings.

#include "SmoothCameraActorComponent.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

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
    //Get pointer to SpringArmComponent 
    AActor* Owner = GetOwner();
    if (Owner) {
      TArray<UActorComponent*> FoundComponennts =
          Owner->GetComponentsByClass(USpringArmComponent::StaticClass());
      for (UActorComponent* current_component : FoundComponennts) {
        USpringArmComponent* finded_component =
            Cast<USpringArmComponent>(current_component);
        if (finded_component) {
          SpringArm = finded_component;
        }
      }
    }

    FOnTimelineFloat TimelineCallback;
    FOnTimelineEventStatic TimelineFinishCallback;

    TimelineCallback.BindUFunction(this, TEXT("ChangeCameraDistanceSmoothlyBegin"));

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

 SmoothCameraMoveTimeline->TickTimeline(DeltaTime);
  
}

void USmoothCameraActorComponent::ChangeCameraDistance(float value) {
  float DistanceValue = value;
  
  if (SmoothCameraMoveTimeline->IsPlaying()) return;
  if (MoveCurve && SpringArm) {
    DistanceValue > 0 ? bIsSmoothCameraReversed = true
                      : bIsSmoothCameraReversed = false;

    // Clamp distance
    float MaxClampDistanceFromPlayer = 1200.f;
    float MinClampDistanceToPlayer = 200.f;

    // Do not change distance if it already too close to player
    if (DistanceValue > 0 &&
        SpringArm->TargetArmLength < MinClampDistanceToPlayer) {
      return;
    }
    // Do not change distance if it already too far from player
    if (DistanceValue < 0 &&
        SpringArm->TargetArmLength > MaxClampDistanceFromPlayer) {
      return;
    }

    SmoothCameraMoveTimeline->SetPlaybackPosition(0, true);
    SmoothCameraMoveTimeline->Play();
  }
}

void USmoothCameraActorComponent::ChangeCameraDistanceSmoothlyBegin() {
  ACharacter* Parent = Cast<ACharacter>(GetOwner());
  if (SpringArm) {
    float PlaybackPosition = SmoothCameraMoveTimeline->GetPlaybackPosition();
    float CurveValue = MoveCurve->GetFloatValue(PlaybackPosition);
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
                                     //FString::SanitizeFloat(CurveValue));
    if (bIsSmoothCameraReversed) {
      CurveValue = -CurveValue;
    }
    SpringArm->TargetArmLength += CurveValue;
  }
}

void USmoothCameraActorComponent::ChangeCameraDistanceSmoothlyEnd() {
  SmoothCameraMoveTimeline->Stop();
}
