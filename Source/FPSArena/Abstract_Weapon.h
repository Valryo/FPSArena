// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Abstract_Weapon.generated.h"

namespace EWeapon
{
	enum State
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
	};

	enum AmmoType
	{
		EBullet,
		ERocket,
		EMax,
	};

	
}

UENUM(BlueprintType)
enum class WeaponClass : uint8
{
	WC_Auto		UMETA(DisplayName = "Automatic"),
	WC_SemiAuto	UMETA(DisplayName = "Semi Automatic"),
	WC_Burst	UMETA(DisplayName = "Burst Firing"),
	WC_Shotgun	UMETA(DisplayName = "Shotgun")
};

UCLASS(abstract)
class FPSARENA_API AAbstract_Weapon : public AActor
{
	GENERATED_BODY()

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

public:	
	// Sets default values for this actor's properties
	AAbstract_Weapon();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AAbstract_Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

protected:
	bool AimingDownSight;
	bool PendingReload;

	/** current weapon state */
	EWeapon::State CurrentState;

	/** update weapon state */
	void SetWeaponState(EWeapon::State NewState);

	/** get current weapon state */
	EWeapon::State GetCurrentState() const;

	/** check if weapon can be reloaded */
	bool CanReload() const;

	/** check if weapon can fire */
	bool CanFire() const;

	/** timer between shots */
	FTimerHandle RefireTimerHandle;

	/** last time the weapon fired */
	float LastFireTime;

	// ---------------------------------------------
	// -===- Properties editable in the editor -===-
	// ---------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Protperties")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Protperties")
		float FireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Protperties")
		float BulletVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Protperties")
		WeaponClass WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
		float ShortReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
		float LongReloadTime;

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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool StartFiring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool StopFiring();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "Weapon")
		bool Reload();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool ToggleAim();

	/** Fires a projectile. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void FireWeapon();
};
