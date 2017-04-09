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
	if (ParticleComp)
	{
		//ParticleComp->Deactivate();
	}

	if (Role == ROLE_Authority)
	{
		if (!OtherActor->IsA(AAbstract_Projectile::StaticClass()))
		{
			AController* Controller = Cast<AController>(Instigator->GetController());
			if (Controller != nullptr)
			{
				if (OtherActor != nullptr)
				{
					// Create a damage event  
					TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
					FDamageEvent DamageEvent(ValidDamageTypeClass);

					OtherActor->TakeDamage(OtherComp->GetName().Compare("Sphere") ? Damage : Damage * HeadshotMultiplier, DamageEvent, Controller, this);
				}
			}

			DisableAndDestroy();
		}
	}

	// play FX locally
	if (GetNetMode() != NM_DedicatedServer)
	{
		/*const FVector EndTrace = Origin + ShootDir * InstantConfig.WeaponRange;
		const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

		SpawnTrailEffect(EndPoint);*/
		SpawnImpactEffects(SweepResult);
	}
}

void AAbstract_Projectile::DisableAndDestroy()
{
	MovementComp->StopMovementImmediately();
	Destroy();
}

void AAbstract_Projectile::InitProjectileProperties(int32 Damage, float HeadshotMultiplier, float Velocity, float Lifespan)
{
	this->Damage = Damage;
	this->HeadshotMultiplier = HeadshotMultiplier;
	this->Velocity = Velocity;
	this->Lifespan = Lifespan;

	InitVelocity(Velocity);

	SetLifeSpan(Lifespan);
}

void AAbstract_Projectile::SpawnImpactEffects(const FHitResult& Impact)
{
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, Impact.GetActor().GetName());
	if (ImpactTemplate)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "SpawnImpactEffects2");
		//FHitResult UseImpact = Impact;

		////// trace again to find component lost during replication
		////if (!Impact.Component.IsValid())
		////{
		////	const FVector StartTrace = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
		////	const FVector EndTrace = Impact.ImpactPoint - Impact.ImpactNormal * 10.0f;
		////	FHitResult Hit = WeaponTrace(StartTrace, EndTrace);
		////	UseImpact = Hit;
		////}

		//FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint);
		//AImpactEffect* EffectActor = GetWorld()->SpawnActorDeferred<AImpactEffect>(ImpactTemplate, SpawnTransform);
		//if (EffectActor)
		//{
		//	EffectActor->SurfaceHit = UseImpact;
		//	UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		//}
	}
}

void AAbstract_Projectile::SetOrigin(const FVector& Origin)
{
	if (ParticleComp)
	{
		FVector Source, End;

		ParticleComp->SetBeamSourcePoint(0, Origin, 0);
		ParticleComp->GetBeamSourcePoint(0, 0, Source);
		ParticleComp->GetBeamEndPoint(0, End);

		/*GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Source" + Source.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "End" + End.ToString());*/
	}
}