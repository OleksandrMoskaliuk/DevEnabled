// Fill out your copyright notice in the Description page of Project Settings.

#include "CrosshairWidget.h"

#include "Blueprint\UserWidget.h"
#include "Components\Image.h"
#include "Runtime/UMG/Public/UMG.h"

UCrosshairWidget::UCrosshairWidget(const FObjectInitializer& ObjectInitializer)
    : UUserWidget(ObjectInitializer) {}

void UCrosshairWidget::NativeConstruct() { Super::NativeConstruct(); }

void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry,
                                  float InDeltaTime) {
  GEngine->AddOnScreenDebugMessage(234, 2, FColor::Blue,
                                   FString("UCrosshairWidget"));
}
