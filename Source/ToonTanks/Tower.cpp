// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (InFireRange()) {
        RotateTurret(tank->GetActorLocation());
    }
}

void ATower::BeginPlay() {
    Super::BeginPlay();

    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &ATower::CheckFireCondition, fireRate, true);
}

void ATower::CheckFireCondition() {
    if (InFireRange()) {
        Fire();
    }
}

bool ATower::InFireRange() {
     if (tank) {
        float distance = FVector::Dist(GetActorLocation(), tank->GetActorLocation());
        return distance <= fireRange;
    }
    return false;
}