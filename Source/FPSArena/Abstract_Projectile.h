// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImpactEffect.h"

#include "GameFramework/Actor.h"
#include "Abstract_Projectile.generated.h"

UCLASS(Abstract, Blueprintable)
class FPSARENA_API AAbstract_Projectile : public AActor
{
	GENERATED_BODY()

	AAbstract_Projectile();

public:
	/** initial setup */
	virtual void PostInitializeComponents() override;

	/** setup velocity */
	void InitVelocity(float Speed);

	/** initialize the projectile properties */
	void InitProjectileProperties(int32 Damage, float HeadshotMultiplier, float Velocity, float Lifespan);

	/** spawn trail effect */
	void SetOrigin(const FVector& Origin);

	/** handle hit */
	UFUNCTION()
		void OnImpact(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		UParticleSystemComponent* ParticleComp;

	/*UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* MeshComp;*/

protected:
	/** impact effects */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		TSubclassOf<AImpactEffect> ImpactTemplate;


	/** shutdown projectile and prepare for destruction */
	void DisableAndDestroy();

	/** spawn effects for impact */
	void SpawnImpactEffects(const FHitResult& Impact);

protected:
	/** Returns MovementComp subobject **/
	FORCEINLINE UProjectileMovementComponent* GetMovementComp() const { return MovementComp; }
	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ParticleComp subobject **/
	FORCEINLINE UParticleSystemComponent* GetParticleComp() const { return ParticleComp; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		int32 Damage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		float HeadshotMultiplier = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		float Velocity = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		float Lifespan = 1.5f;
};
