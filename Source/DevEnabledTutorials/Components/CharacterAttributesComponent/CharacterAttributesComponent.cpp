// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributesComponent.h"

#include "Components\ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework\HUD.h"
#include "../../HUD/HealthBar/DEV_HealthBar.h"
#include "Blueprint/UserWidget.h"
#include "../../Characters/MainCharacter/MainCharacter.h"
#include "Components/SphereComponent.h"

    // Sets default values for this component's properties
UCharacterAttributesComponent::UCharacterAttributesComponent() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = false;
  /* HUD */
  PlayerHealthBarHUDclass = nullptr;
  PlayerHealthBarHUD = nullptr;
  /* Attribute defaults params */
  DefaultMaxHealth = 100;
  CurrentHealth = 100;

  // ...
}

void UCharacterAttributesComponent::TakeDamage(AActor* DamagedActor,
                                               float Damage,
                                               const UDamageType* DamageType,
                                               AController* InstigatedBy,
                                               AActor* DamageCauser) 
{
  if (PlayerHealthBarHUD) {
    CurrentHealth = -0.01;
    PlayerHealthBarHUD->SetHealth(CurrentHealth, DefaultMaxHealth);
  }
}

void UCharacterAttributesComponent::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) {
 /* GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red,
                                   "Begin overlap Dev attributes!");*/
  if (PlayerHealthBarHUD) {
    CurrentHealth += 10;
    PlayerHealthBarHUD->SetHealth(CurrentHealth, DefaultMaxHealth);

    GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
                                     FString::SanitizeFloat(CurrentHealth));
  }


}

// Called when the game starts
void UCharacterAttributesComponent::BeginPlay() {
  Super::BeginPlay();
  AActor* Owner = GetOwner();
  /*Show player Health bar */
  if (PlayerHealthBarHUDclass) {
    
    auto PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    check(PController);
    PlayerHealthBarHUD = CreateWidget<UDEV_HealthBar>(
        GetWorld(), PlayerHealthBarHUDclass, FName("HealthBar Widget"));
    // check(PController);
    if (PlayerHealthBarHUD) {
      PlayerHealthBarHUD->AddToPlayerScreen();
      PlayerHealthBarHUD->SetHealth(CurrentHealth, DefaultMaxHealth);
    }
  }
  
  GetOwner()->OnTakeAnyDamage.AddDynamic(
      this, &UCharacterAttributesComponent::TakeDamage);

  
  if (Owner) {
    AMainCharacter* MCharacter = Cast<AMainCharacter>(Owner);
    if (MCharacter && MCharacter->SphereOverlapComponent) {
      MCharacter->SphereOverlapComponent->OnComponentBeginOverlap.AddDynamic(
          this, &UCharacterAttributesComponent::OnOverlapBegin);
    }
  }
     
  // ...
}

void UCharacterAttributesComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason) {
  if (PlayerHealthBarHUD) {
    PlayerHealthBarHUD->RemoveFromParent();
    PlayerHealthBarHUD = nullptr;
  }
}

// Called every frame
void UCharacterAttributesComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}
