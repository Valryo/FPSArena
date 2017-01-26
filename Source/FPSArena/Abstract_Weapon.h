// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Abstract_Weapon.generated.h"

UCLASS(abstract)
class FPSARENA_API AAbstract_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbstract_Weapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float FireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float BulletVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
		float MagazineSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
		float NbAmmoInMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
		float NbAmmoInReserve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float VerticalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float HorizontalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float Bloom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float ConeOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float AccuracyWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float AccuracyStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float AccuracyCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float AccuracyCrouchWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
		float AccuracyJumping;
		
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual bool Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual bool Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual bool Aim();

};
