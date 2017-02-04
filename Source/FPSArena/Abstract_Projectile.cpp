// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Projectile.h"


// Sets default values
AAbstract_Projectile::AAbstract_Projectile()
{
	Damage = 10;
	Velocity = 60000.f;
	Lifespan = 1.f;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAbstract_Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = Velocity;
	ProjectileMovement->MaxSpeed = Velocity;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 1 second by default
	InitialLifeSpan = Lifespan;
}

void AAbstract_Projectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "On Hit");

	Destroy();*/
}

void AAbstract_Projectile::InitProjectileProperties(int32 Damage, float Velocity, float Lifespan)
{
	this->Damage = Damage;
	this->Velocity = Velocity;
	this->Lifespan = Lifespan;

	ProjectileMovement->InitialSpeed = Velocity;
	ProjectileMovement->MaxSpeed = Velocity;

	SetLifeSpan(Lifespan);
}