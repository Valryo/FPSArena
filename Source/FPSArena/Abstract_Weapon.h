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
	WC_Burst	UMETA(DisplayName = "Burst Firing")
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
		class USkeletalMeshComponent* FP_SightSocket;

public:	
	// Sets default values for this actor's properties
	AAbstract_Weapon();

	/** initial setup */
	virtual void PostInitializeComponents() override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AAbstract_Projectile> ProjectileClass;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	bool AimingDownSight;
	bool PendingReload;
	bool IsEquipped;
	bool PendingEquip;
	bool WantsToFire;
	bool Refiring;
	bool Bursting;
	

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

	/** consume a ammo */
	void UseAmmo();

	/** determines the actual weapon state */
	void DetermineWeaponState();

	/** starts firing */
	void OnBurstStarted();

	/** stops firing */
	void OnBurstFinished();

	/** handles firing */
	void HandleFiring();

	/** [server] fire & update ammo */
	UFUNCTION(reliable, server, WithValidation)
		void ServerHandleFiring();

	/** reloads the weapon */
	void ReloadWeapon();

	/** Handle for efficient management of StopReload timer */
	FTimerHandle TimerHandle_StopReload;

	/** Handle for efficient management of ReloadWeapon timer */
	FTimerHandle TimerHandle_ReloadWeapon;

	/** timer between shots */
	FTimerHandle RefireTimerHandle;

	/** last time the weapon fired */
	float LastFireTime;

	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		FName MuzzleAttachPoint;

	/** times in second between two consecutive shots */
	float TimeBetweenShots;

	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

	/** play weapon sounds */
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);


	FVector GetCameraAim() const;

	float GetReloadDuration();

	int BurstCounter = 0;

	/** current spread from continuous firing */
	float CurrentFiringSpread = 0.f;


	//////////////////////////////////////////////////////////////////////////
	// Weapon utils
	FVector InitialRotation;

	/** current spread from continuous firing */
	float CurrentFiringSpread = 0.f;

	/** Computes the horizontal recoil */
	float ComputeHorizontalRecoil();

	/** Computes the spread */
	FVector ComputeSpread(const FVector& ShootDir);


	//////////////////////////////////////////////////////////////////////////
	// Input - server side

	UFUNCTION(reliable, server, WithValidation)
		void ServerStartFire();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStopFire();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStartReload();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStopReload();

	/** spawn projectile on server */
	UFUNCTION(reliable, server, WithValidation)
		void ServerFireProjectile(FVector Origin, FVector ShootDir);

	/** Called in network play to do the cosmetic fx for firing */
	virtual void SimulateWeaponFire();

	/** Called in network play to stop cosmetic fx (e.g. for a looping shot). */
	virtual void StopSimulatingWeaponFire();

public:
	/** set the weapon's owning pawn */
	void SetOwningPawn(ACharacter* NewOwner);

	


protected:

	/** Damage dealt by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		int32 Damage;

	/** Fire rate in rounds per minutes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		float FireRate;
	
	/** Time between two consecutive shot while burst firing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		float TimeBetweenShotBurstFire;

	/** Time between two consecutive shot while burst firing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		float NumberBurstShot;

	/** Projectile velocity in meters per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		float ProjectileVelocity;

	/** Projectile LifeSpan in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		float ProjectileLifeSpan;

	/** Class of the weapon : auto, semi-auto, burst */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
		WeaponClass WeaponClass;

	/** Reload time in seconds when there's still a bullet in the magazine */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
		float ShortReloadTime;

	/** Reload time in seconds when there's no bullet in the magazine */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
		float LongReloadTime;

	/** Number of rounds in the magazine */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
		int MagazineSize;
	
	/** Amount of bullets left in the magazine */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Magazine")
	int CurrentAmmoInClip;

	/** Amount of bullet left in the reserve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Magazine")
		int CurrentAmmoInReserve;
	
	/** Number of ammunition */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
		int MaxAmmo;

	/** Base weapon spread (degrees) */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Spread")
		float WeaponSpread;

	/** Continuous firing: spread increment */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Spread")
		float FiringSpreadIncrement;

	/** Continuous firing: max increment */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Spread")
		float FiringSpreadMax;

	/** Vertical recoil in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy|Recoil")
		float VerticalRecoil;
	
	float HorizontalRecoil = 0.f;

	/** Horizontal recoil minimum in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy|Recoil")
		float HorizontalRecoilMin;
	
	/** Horizontal recoil maximum in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy|Recoil")
		float HorizontalRecoilMax;

	/** Horizontal recoil tolerance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy|Recoil")
		float HorizontalTolerance;

	/** Angle of recoil minimum  in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy|Recoil")
		float AngleMin;

	/** Angle or recoil maximum in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy|Recoil")
		float AngleMax;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
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
		float AccuracyJumping;*/

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// -===- Sound effect -===-

	/** firing audio (bLoopedFireSound set) */
	UPROPERTY(Transient)
		UAudioComponent* FireAC;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* FireSound;

	/** looped fire sound (bLoopedFireSound set) */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* FireLoopSound;

	/** finished burst sound (bLoopedFireSound set) */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* FireFinishSound;

	/** out of ammo sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* OutOfAmmoSound;

	/** reload sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* ReloadSound;

	/** is fire sound looped? */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		bool LoopedFireSound = true;

public :
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StartFiring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StopFiring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StartReloading();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StopReloading();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool ToggleAim();

	/** Fires a projectile. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void FireWeapon();

};
