// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Abstract_Projectile.generated.h"

UCLASS(abstract)
class FPSARENA_API AAbstract_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbstract_Projectile();

	// ---------------------------------------------
	// -===- Properties editable in the editor -===-
	// ---------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet property")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet property")
		float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet property")
		float Lifespan;

	/** Projectile movement component **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovement;

	/** inits velocity of the projectile in the shoot direction */
	void InitVelocity(const FVector& ShootDirection);
};
