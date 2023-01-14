// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "CrosshairWidget.generated.h"

/**
 *
 */
UCLASS()
class DEVENABLEDTUTORIALS_API UCrosshairWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  UCrosshairWidget(const FObjectInitializer& ObjectInitializer);

  virtual void NativeConstruct() override;
  virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  class UImage* BaseCrosshair;

 private:
};
