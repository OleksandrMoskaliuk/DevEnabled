// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHeadUpDisplay.generated.h"

/**
 *
 */
UCLASS()
class DEVENABLEDTUTORIALS_API AMainHeadUpDisplay : public AHUD {
  GENERATED_BODY()

 public:
  AMainHeadUpDisplay();
  /* For  UUserWidget clases only */
  UPROPERTY(EditDefaultsOnly, Category = "User Widgets")
  TArray<TSubclassOf<UUserWidget>> UserWidgets;

 private:
  // Current list of created UI widgets that are constantly active on screen
  TArray<class UUserWidget*> m_createdWidgets;

 protected:
  virtual void BeginPlay() override;
  virtual void DrawHUD() override;
  virtual void Tick(float DeltaSeconds) override;
};
