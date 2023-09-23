// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableObject.h"

// Sets default values
APickableObject::APickableObject() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APickableObject::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void APickableObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void APickableObject::OnInteract_Implementation() {}

void APickableObject::OnImpulse_Implementation(FVector Impulse) {}

void APickableObject::StartFocus_Implementation() {}

void APickableObject::EndFocus_Implementation() {}

