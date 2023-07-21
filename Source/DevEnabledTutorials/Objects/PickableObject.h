// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Interfaces\DEV_Interact.cpp"
#include "PickableObject.generated.h"

UCLASS()
class DEVENABLEDTUTORIALS_API APickableObject : public AActor,
                                                public IDEV_Interact {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  APickableObject();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void OnInteract();
  void OnInteract_Implementation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void StartFocus();
  void StartFocus_Implementation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void EndFocus();
  void EndFocus_Implementation();
};
