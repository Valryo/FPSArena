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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		flaot Lifespan;
};
