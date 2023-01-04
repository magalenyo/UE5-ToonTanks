// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank() {
    springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
    springArm->SetupAttachment(RootComponent);

    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera component"));
    camera->SetupAttachment(springArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (playerController) {
        FHitResult hitResult;
        playerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);
        DrawDebugSphere(
            GetWorld(),
            hitResult.ImpactPoint,
            25.f,
            12,
            FColor::Red,
            false,
            -1.f
        );

        RotateTurret(hitResult.ImpactPoint);
    }
}

void ATank::BeginPlay() {
    Super::BeginPlay();

    playerController = Cast<APlayerController>(GetController());
}


void ATank::Move(float value) {
    FVector deltaLocation = FVector::ZeroVector;
	deltaLocation.X = value * speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(deltaLocation, true);
}

void ATank::Turn(float value) {
    FRotator deltaRotation = FRotator::ZeroRotator;
    deltaRotation.Yaw = value * turnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(deltaRotation, true);
}