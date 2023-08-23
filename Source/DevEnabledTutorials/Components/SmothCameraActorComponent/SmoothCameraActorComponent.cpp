// Fill out your copyright notice in the Description page of Project Settings.

#include "SmoothCameraActorComponent.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../Characters/MainCharacter/MainCharacter.h"

// Sets default values for this component's properties
USmoothCameraActorComponent::USmoothCameraActorComponent() : play_counter(0) {
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
    // Get pointer to SpringArmComponent
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

    TimelineCallback.BindUFunction(this,
                                   TEXT("ChangeCameraDistanceSmoothlyBegin"));
    TimelineFinishCallback.BindUFunction(
        this, TEXT("ChangeCameraDistanceSmoothlyEnd"));

    SmoothCameraMoveTimeline->AddInterpFloat(MoveCurve, TimelineCallback);
    SmoothCameraMoveTimeline->SetTimelineFinishedFunc(TimelineFinishCallback);
  }

  /* Binding delegates for controll camera distance */
  AActor* Owner = GetOwner();
  if (Owner) {
    AMainCharacter* MainCharacter = Cast<AMainCharacter>(Owner);
    if (MainCharacter) {
      /*MainCharacter->ICD_Delegate.BindUObject(
          this, &USmoothCameraActorComponent::IncreaseCameraDistance);

      MainCharacter->DCD_Delegate.BindUObject(
          this, &USmoothCameraActorComponent::DecreaseCameraDistance);*/
     
      MainCharacter->CCD_Delegate.AddUObject(
          this, &USmoothCameraActorComponent::ChangeCameraDistance);
       
    }
  }
}

// Called every frame
void USmoothCameraActorComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

  SmoothCameraMoveTimeline->TickTimeline(DeltaTime);
}

void USmoothCameraActorComponent::ChangeCameraDistance(float DistanceValue) {

 GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,
                                   "Dinamic binding works!!!!!!!");

  // if (SmoothCameraMoveTimeline->IsPlaying()) return;
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

    if (SmoothCameraMoveTimeline->IsPlaying()) {
      SmoothCameraMoveTimeline->PlayFromStart();
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

    if (bIsSmoothCameraReversed) {
      CurveValue = -CurveValue;
    }
    SpringArm->TargetArmLength += CurveValue;
  }
}

void USmoothCameraActorComponent::ChangeCameraDistanceSmoothlyEnd() {
  SmoothCameraMoveTimeline->Stop();
}

void USmoothCameraActorComponent::IncreaseCameraDistance() {
 ChangeCameraDistance(1);
}

void USmoothCameraActorComponent::DecreaseCameraDistance() {
 ChangeCameraDistance(-1);
}
