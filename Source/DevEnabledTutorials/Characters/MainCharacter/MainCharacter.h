// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FControlCameraDistanceDelegate, float);

class UInputAction;

UCLASS()
class DEVENABLEDTUTORIALS_API AMainCharacter : public ACharacter {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
  class UDEV_SpringArmComponent* DEV_SpringArmComponent;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
  class UDEV_CameraComponent* DEV_CameraComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
  class UStaticMeshComponent* StaticMeshComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
  float BaseTurnRate;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
  float BaseLookUpAtRate;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
  class UInputMappingContext* InputMappingContext;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
  TMap<FString, UInputAction*> InputActionsMap;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
  class USphereComponent* SphereOverlapComponent;

  // Sets default values for this character's properties
  AMainCharacter();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void MoveForward();
  void MoveRight(float Value);
  void LookUpAtRate(float Value);
  void TurnUpAtRate(float Value);

  // Handle move input
  void Move(const FInputActionValue& Value);

  // Handle look input
  void Look(const FInputActionValue& Value);

  // Handle jump input
  void Jump(const FInputActionValue& Value);

  // Fire line trace
  void FireLineTrace(const FInputActionValue& Value);

  // Default Player Interact
  void PlayerInteract(const FInputActionValue& Value);

 public:
  // Handle camera distance input
  UFUNCTION()
  void ChangeCameraDistance(const FInputActionValue& Value);

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent* PlayerInputComponent) override;

  /* Change camera distance delegate for ChangeCameraDistance(const
  FInputActionValue& Value) For maling MainCharacter class independent from
  SmoothCameraComponent creation, (now optional) when you add component you have
  fonctionality no force creation any more!! */
  FControlCameraDistanceDelegate CCD_Delegate;

 private:
  // Line trace helper function
  AActor* FindAcotrByLineTrace(bool ShowLine = false);

  AActor* FocusedActor;

  UFUNCTION()
  void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
                    AActor* OtherActor, UPrimitiveComponent* OtherComp,
                    int32 OtherBodyIndex, bool bFromSweep,
                    const FHitResult& SweepResult);
};
