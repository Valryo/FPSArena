// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void InitVelocity(FVector& ShootDirection);

	/** handle hit */
	UFUNCTION()
		void OnImpact(const FHitResult& HitResult);

private:
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UParticleSystemComponent* ParticleComp;

	/*UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* MeshComp;*/

protected:
	/** shutdown projectile and prepare for destruction */
	void DisableAndDestroy();

protected:
	/** Returns MovementComp subobject **/
	FORCEINLINE UProjectileMovementComponent* GetMovementComp() const { return MovementComp; }
	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ParticleComp subobject **/
	FORCEINLINE UParticleSystemComponent* GetParticleComp() const { return ParticleComp; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties")
		float Lifespan;
};
