// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributesComponent.generated.h"


UCLASS(ClassGroup = ("Character Components"),
       meta = (BlueprintSpawnableComponent))
class DEVENABLEDTUTORIALS_API UCharacterAttributesComponent
    : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UCharacterAttributesComponent();

  UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
  float DefaultMaxHealth;

  UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
  float CurrentHealth;

  /** UDEV_HealthBar Blueprint  **/
  UPROPERTY(EditAnywhere)
  TSubclassOf<class UDEV_HealthBar> PlayerHealthBarHUDclass;

  UPROPERTY()
  class UDEV_HealthBar* PlayerHealthBarHUD;

  


 protected:
  // Called when the game starts
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UFUNCTION()
  void TakeDamage(AActor* DamagedActor, float Damage,
                  const class UDamageType* DamageType,
                  class AController* InstigatedBy, AActor* DamageCauser);

  UFUNCTION()
  void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                    AActor* OtherActor, UPrimitiveComponent* OtherComp,
                    int32 OtherBodyIndex, bool bFromSweep,
                    const FHitResult& SweepResult);

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;
};
