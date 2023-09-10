// Fill out your copyright notice in the Description page of Project Settings.

#include "DEV_CameraComponent.h"

#include "../../Characters/MainCharacter/MainCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

UDEV_CameraComponent::UDEV_CameraComponent() : SpringArm(nullptr) {  }

void UDEV_CameraComponent::BeginPlay() {
  Super::BeginPlay();
  // Get pointer to SpringArmComponent
  AActor* Owner = GetOwner();
  if (Owner) {
    TSet<UActorComponent*> FoundComponennts = Owner->GetComponents();
    for (UActorComponent* current_component : FoundComponennts) {
      USpringArmComponent* finded_component =
          Cast<USpringArmComponent>(current_component);
      IsValid(finded_component) ? SpringArm = finded_component
                                : SpringArm = nullptr;
      
    }
  }
}

void UDEV_CameraComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// USpringArmComponent* SpringArmTemp = Cast<USpringArmComponent>(Owner);
// SpringArm = SpringArmTemp;

void UDEV_CameraComponent::ChangeCameraDistance(float Direction) {
 
}
