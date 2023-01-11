#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "MyInputConfigData.generated.h"

/**
 * Store pointer to Input Action for native binding
 */
UCLASS()
class DEVENABLEDTUTORIALS_API UMyInputConfigData : public UDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  UInputAction* InputMove;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  UInputAction* InputLook;
};