// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	projectileMovementComponent->MaxSpeed = 1300.f;
	projectileMovementComponent->InitialSpeed = 1300.f;

	trailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke trail"));
	trailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (launchSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			launchSound,
			GetActorLocation()
		);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	AActor* myOwner = GetOwner();
	if (myOwner == nullptr) {
		Destroy();

		return;
	}

	AController* myOwnerInstigator = myOwner->GetInstigatorController();
	UClass* damageType = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != myOwner) {
		UGameplayStatics::ApplyDamage(
			OtherActor,
			damage,
			myOwnerInstigator,
			this,
			damageType
		);
		
		if (hitParticles) {
			UGameplayStatics::SpawnEmitterAtLocation(
				this,
				hitParticles,
				GetActorLocation(),
				GetActorRotation()
			);
		}

		if (hitSound) {
			UGameplayStatics::PlaySoundAtLocation(
				this,
				hitSound,
				GetActorLocation()
			);
		}

		if (hitCameraShakeClass) {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(hitCameraShakeClass);
		}
	}

	Destroy();
}