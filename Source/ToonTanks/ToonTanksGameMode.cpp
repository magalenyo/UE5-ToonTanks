// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"

void AToonTanksGameMode::ActorDied(AActor* deadActor) {
    if (deadActor == tank) {
        tank->HandleDestruction();
        
        if (tank->GetPlayerController()) {
            tank->DisableInput(tank->GetPlayerController());
            tank->GetPlayerController()->bShowMouseCursor = false;
        }
    }
    else if (ATower* destroyedTower = Cast<ATower>(deadActor)) {
        destroyedTower->HandleDestruction();
    }
}

void AToonTanksGameMode::BeginPlay() {
    Super::BeginPlay();

    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}