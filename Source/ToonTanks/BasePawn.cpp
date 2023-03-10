// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsuleComp;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(capsuleComp);

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	turretMesh->SetupAttachment(baseMesh);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn point"));
	projectileSpawnPoint->SetupAttachment(turretMesh);
}

void ABasePawn::HandleDestruction() {
	if (deathParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			deathParticles,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	if (deathSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			deathSound,
			GetActorLocation()
		);
	}

	if (deathCameraShakeClass) {
		if (deathCameraShakeClass) {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(deathCameraShakeClass);
		}
	}
}

void ABasePawn::RotateTurret(FVector lookAtTarget) {
	FVector toTarget = lookAtTarget - turretMesh->GetComponentLocation();
	FRotator lookAtRotation = FRotator(0.f, toTarget.Rotation().Yaw, 0.f);
	turretMesh->SetWorldRotation(
		FMath::RInterpTo(
			turretMesh->GetComponentRotation(), 
			lookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			25.f)
		);
}

void ABasePawn::Fire() {
	FVector location = projectileSpawnPoint->GetComponentLocation();
	FRotator rotation = projectileSpawnPoint->GetComponentRotation();

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, location, rotation);
	projectile->SetOwner(this);
}