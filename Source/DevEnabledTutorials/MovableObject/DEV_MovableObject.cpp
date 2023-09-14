// Fill out your copyright notice in the Description page of Project Settings.


#include "DEV_MovableObject.h"

// Sets default values
ADEV_MovableObject::ADEV_MovableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
        MeshComponent =
            CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
        MeshComponent->SetupAttachment(GetRootComponent());
}

void ADEV_MovableObject::OnInteract_Implementation() 
{
      
        //MeshComponent->AddImpu

}

void ADEV_MovableObject::OnImpulse_Implementation(FVector Impulse) 
{
        
        MeshComponent->AddImpulse(Impulse * MeshComponent->GetMass());
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "__FUNCTION__");
}

void ADEV_MovableObject::StartFocus_Implementation() {}

void ADEV_MovableObject::EndFocus_Implementation() {}

// Called when the game starts or when spawned
void ADEV_MovableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADEV_MovableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

