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

	// ---------------------------------------------
	// -===- Properties hidden from the editor -===-
	// ---------------------------------------------
	UPROPERTY()
		bool AimingDownSight;

	// ---------------------------------------------
	// -===- Properties editable in the editor -===-
	// ---------------------------------------------
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
		

	// ---------------------------------------------
	// -===-  Methods editable in the editor   -===-
	// ---------------------------------------------
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
		bool Fire();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "Weapon")
		bool Reload();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool ToggleAim();

};
