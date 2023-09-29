// Fill out your copyright notice in the Description page of Project Settings.

#include "MainHeadUpDisplay.h"

#include "../CrosshairWidget\CrosshairWidget.h"
#include "Components/WidgetComponent.h"

AMainHeadUpDisplay::AMainHeadUpDisplay() {}

void AMainHeadUpDisplay::DrawHUD() { AHUD::DrawHUD(); }

void AMainHeadUpDisplay::BeginPlay() {
  Super::BeginPlay();
  GEngine->AddOnScreenDebugMessage(234, 2, FColor::Blue,
                                   FString("AMainHeadUpDisplay::BeginPlay"));
  // If any widgets need to be added
  // Iterate through all widgets
  for (TSubclassOf<UUserWidget>& widget : UserWidgets) {
    GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Yellow, FString("widget"));
    // Pass if index is empty
    if ((Cast<UClass>(widget)) != nullptr) {
      // Iterate through all widgets
      // Create an instance of the widget and add to viewport
      UUserWidget* createdWidget =
          CreateWidget<UUserWidget>(GetWorld(), *widget);
      if (createdWidget != nullptr) {
        createdWidget->AddToViewport();

        // Store instanced widget in array
        m_createdWidgets.Add(createdWidget);
      }
    }
  }
}

void AMainHeadUpDisplay::Tick(float DeltaSeconds) { Super::Tick(DeltaSeconds); }
