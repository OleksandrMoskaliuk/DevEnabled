// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "MainCharacter.generated.h"


UCLASS()
class DEVENABLEDTUTORIALS_API AMainCharacter : public ACharacter {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
  class USpringArmComponent* SpringArmComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
  class UCameraComponent* CameraComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
  class UStaticMeshComponent* StaticMeshComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
  float BaseTurnRate;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
  float BaseLookUpAtRate;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
  class UInputMappingContext* InputMapping;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
  class UMyInputConfigData* InputActions;

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

  

 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent* PlayerInputComponent) override;

};
