// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "DevEnabledTutorials\Interfaces\DEV_Interact.h"
#include "DEV_FloatingObject.generated.h"

UCLASS()
class DEVENABLEDTUTORIALS_API ADEV_FloatingObject : public AActor,
                                                    public IDEV_Interact {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  ADEV_FloatingObject();
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Mesh")
  class UStaticMeshComponent *ItemMesh;

 protected:

  FTimeline FloatingItemTimeline;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floating Item")
  float ZOffset;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline")
  class UCurveFloat *FloatingItemCurve;

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void OnInteract();
  void OnInteract_Implementation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
  void OnInteractWithActor(AActor *Other);
  void OnInteractWithActor_Implementation(AActor *Other);

 private:

  // Floating Item mechanics
  FVector FloatingItemStartLocation;
  FVector FloatingItemEndLocation;
  UFUNCTION()
  void FloatingItemProgress(float CurveValue);
  
};
