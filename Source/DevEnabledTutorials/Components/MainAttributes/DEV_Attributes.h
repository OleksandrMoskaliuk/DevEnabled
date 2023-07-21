// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DEV_Attributes.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEVENABLEDTUTORIALS_API UDEV_Attributes : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UDEV_Attributes();

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
  float DefaultHealth;
    
  UPROPERTY(BlueprintReadOnly)
  float Health;

  UPROPERTY(EditAnywhere)
  TSubclassOf<class UDEV_HealthBar> PlayerHealthBarHUDclass;

  UPROPERTY()
  class UDEV_HealthBar* PlayerHealthBarHUD;

  UFUNCTION()
  void TakeDamage(AActor* DamagedActor, float Damage,
                  const class UDamageType* DamageType,
                  class AController* InstigatedBy, AActor* DamageCauser);

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;

 
};
