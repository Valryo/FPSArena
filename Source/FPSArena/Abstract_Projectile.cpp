// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Projectile.h"


// Sets default values
AAbstract_Projectile::AAbstract_Projectile()
{
	Damage = 10;
	Velocity = 10.f;
	Lifespan = 1.f;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAbstract_Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Set as root component
	RootComponent = CollisionComp;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = false;
	ParticleComp->bAutoDestroy = false;
	ParticleComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = Velocity;
	MovementComp->MaxSpeed = Velocity;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
}

void AAbstract_Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAbstract_Projectile::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(Instigator);

	SetLifeSpan(Lifespan);
}

void AAbstract_Projectile::InitVelocity(float Speed)
{
	if (MovementComp)
	{
		MovementComp->InitialSpeed = Speed;
		MovementComp->MaxSpeed = Speed;
		MovementComp->Velocity *= Speed;
	}
}

void AAbstract_Projectile::OnImpact(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Projectile : OnImpact " + OtherActor->GetName());

		APlayerController* PlayerController = Cast<APlayerController>(Instigator->GetController());
		if (PlayerController != nullptr)
		{
			if (OtherActor != nullptr)
			{
				// Create a damage event  
				TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
				FDamageEvent DamageEvent(ValidDamageTypeClass);

				OtherActor->TakeDamage(Damage, DamageEvent, PlayerController, this);
			}
		}
		
		DisableAndDestroy();
	}
}

void AAbstract_Projectile::DisableAndDestroy()
{
	MovementComp->StopMovementImmediately();
	Destroy();
}

void AAbstract_Projectile::InitProjectileProperties(int32 Damage, float Velocity, float Lifespan)
{
	this->Damage = Damage;
	this->Velocity = Velocity;
	this->Lifespan = Lifespan;

	InitVelocity(Velocity);

	SetLifeSpan(Lifespan);
}