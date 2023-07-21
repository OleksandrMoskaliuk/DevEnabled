// Fill out your copyright notice in the Description page of Project Settings.

#include "DEV_Attributes.h"

#include "Components\ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework\HUD.h"
#include "../../HUD/HealthBar/DEV_HealthBar.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UDEV_Attributes::UDEV_Attributes() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = false;
  DefaultHealth = 100.f;
  Health = DefaultHealth;
  //PlayerHealthBarHUDclass = ;
  PlayerHealthBarHUD = nullptr;
  // ...
}

// Called when the game starts
void UDEV_Attributes::BeginPlay() {
  Super::BeginPlay();
  
  // ...
  /*APlayerController* PlayerController =
      UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController) {
    AHUD* Ahud = PlayerController->GetHUD();
    if (Ahud) {
      UProgressBar* pb = Cast<UProgressBar>(Ahud);
      pb->Percent = 100.f;
    }
  }*/
  if (PlayerHealthBarHUDclass)
{
    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White,
                                     "Begin play Dev attributes!");
      auto PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    check(PController);
      PlayerHealthBarHUD = CreateWidget<UDEV_HealthBar>(GetWorld() , PlayerHealthBarHUDclass, FName("HealthBar Widget"));
    //check(PController);
      if (PlayerHealthBarHUD) {
      PlayerHealthBarHUD->AddToPlayerScreen();
      }

}



  GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UDEV_Attributes::TakeDamage);
}

void UDEV_Attributes::EndPlay(const EEndPlayReason::Type EndPlayReason) {}

// Called every frame
void UDEV_Attributes::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void UDEV_Attributes::TakeDamage(AActor* DamagedActor, float Damage,
                                 const UDamageType* DamageType,
                                 AController* InstigatedBy,
                                 AActor* DamageCauser) {
 
}
