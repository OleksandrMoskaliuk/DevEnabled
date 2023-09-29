// Fill out your copyright notice in the Description page of Project Settings.

#include "DEV_FloatingObject.h"

#include "DevEnabledTutorials\Characters\MainCharacter\MainCharacter.h"
#include "DevEnabledTutorials\Components\CharacterAttributesComponent\CharacterAttributesComponent.h"
#include "DevEnabledTutorials\HUD\HealthBar\DEV_HealthBar.h"

// Sets default values
ADEV_FloatingObject::ADEV_FloatingObject() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
  ItemMesh->SetupAttachment(GetRootComponent());
  ZOffset = 100;
}

// Called when the game starts or when spawned
void ADEV_FloatingObject::BeginPlay() {
  Super::BeginPlay();

  if (FloatingItemCurve && ItemMesh) {
    FloatingItemStartLocation = FloatingItemEndLocation =
        ItemMesh->GetRelativeLocation();
    FloatingItemEndLocation.Z += ZOffset;

    FOnTimelineFloat FloatingItemFuncWraper;
    FloatingItemFuncWraper.BindUFunction(this, "FloatingItemProgress");
    FloatingItemTimeline.AddInterpFloat(FloatingItemCurve,
                                        FloatingItemFuncWraper);
    FloatingItemTimeline.SetLooping(true);
    FloatingItemTimeline.PlayFromStart();
  }
}

// Called every frame
void ADEV_FloatingObject::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  FloatingItemTimeline.TickTimeline(DeltaTime);
}

void ADEV_FloatingObject::OnInteract_Implementation() {}

void ADEV_FloatingObject::OnInteractWithActor_Implementation(AActor* Other) {
  if (AMainCharacter* MCharacter = Cast<AMainCharacter>(Other)) {
    /*if (UCharacterAttributesComponent* finded_component =
            Cast<UCharacterAttributesComponent>(
                MCharacter->GetRootComponent())) {
      finded_component->PlayerHealthBarHUD->SetHealth(0,
    finded_component->DefaultMaxHealth);
    }*/

    TSet<UActorComponent*> FoundComponennts = MCharacter->GetComponents();
    for (UActorComponent* current_component : FoundComponennts) {
      if (UCharacterAttributesComponent* LookingForComponent =
              Cast<UCharacterAttributesComponent>(current_component)) {

        float MaxHelth = LookingForComponent->DefaultMaxHealth;
        float HealAmount = 10;
        LookingForComponent->PlayerHealthBarHUD->SetHealth(LookingForComponent->CurrentHealth + HealAmount,
                                                           MaxHelth);
         this->Destroy();
      }
    }

    /* if (LookingForComponent) {
      
     }*/
  }
}

void ADEV_FloatingObject::FloatingItemProgress(float CurveValue) {
  FVector NewLocation = FMath::Lerp(FloatingItemStartLocation,
                                    FloatingItemEndLocation, CurveValue);
  ItemMesh->SetRelativeLocation(NewLocation);
}
