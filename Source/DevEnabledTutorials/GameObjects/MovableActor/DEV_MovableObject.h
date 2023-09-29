// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevEnabledTutorials\Interfaces\DEV_Interact.h"
#include "DEV_MovableObject.generated.h"

UCLASS()
class DEVENABLEDTUTORIALS_API ADEV_MovableObject : public AActor,
                                                   public IDEV_Interact {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  ADEV_MovableObject();
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
  UStaticMeshComponent *MeshComponent;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void OnInteract();
  void OnInteract_Implementation();

   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void OnImpulse(FVector Impulse);
  void OnImpulse_Implementation(FVector Impulse);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void StartFocus();
  void StartFocus_Implementation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void EndFocus();
  void EndFocus_Implementation();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
