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

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	WC_LMG		UMETA(DisplayName = "LMG"),
	WC_SMG		UMETA(DisplayName = "SMG"),
	WC_AR		UMETA(DisplayName = "Assault Rifle"),
	WC_Carbine	UMETA(DisplayName = "Carbine"),
	WC_Shotgun	UMETA(DisplayName = "Shotgun")
};

UCLASS(abstract)
class FPSARENA_API AAbstract_Weapon : public AActor
{
	GENERATED_BODY()

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* SightFront;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* SightRear;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UCameraComponent* Camera;

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

	virtual void Tick(float DeltaTime);

	UPROPERTY(Transient, Replicated)
		bool AimingDownSight;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
		bool PendingReload;

	bool IsEquipped;
	bool PendingEquip;
	bool WantsToFire;
	bool Refiring;
	bool Bursting;
	bool Fired;
	bool Recovering;
	

	UFUNCTION()
		void OnRep_Reload();

	/** current weapon state */
	EWeapon::State CurrentState;

	/** update weapon state */
	void SetWeaponState(EWeapon::State NewState);

	/** get current weapon state */
	EWeapon::State GetCurrentState() const;

	/** check if weapon can be reloaded */
	bool CanReload() const;

	/** check if weapon can fire */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
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

	/** Timer for starting to recover*/
	FTimerHandle TimerHandle_StartRecover;

	/** last time the weapon fired */
	float LastFireTime;

	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName SightFrontAttachPoint = "SightFrontSocket";
	
	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName SightRearAttachPoint = "SightRearSocket";

	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName CameraAttachPoint = "CameraSocket";

	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Effect)
		FName MuzzleAttachPoint = "MuzzleFlashSocket";

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* MuzzleFX;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
		UParticleSystemComponent* MuzzlePSC;

	/** times in second between two consecutive shots */
	float TimeBetweenShots;

	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

	/** play weapon sounds */
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);


	FVector GetCameraAim() const;

	float GetReloadDuration();

	float GetImprovedAccuracy(float f);

	/** burst counter, used for replicating fire events to remote clients */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
		int BurstCounter = 0;

	UFUNCTION()
		void OnRep_BurstCounter();

	//////////////////////////////////////////////////////////////////////////
	// Weapon utils
	float HorizontalRecoil = 0.f;

	float RecoveryX = 0.f, RecoveryY = 0.f;
	float CurrentRecoveryX = 0.f, CurrentRecoveryY = 0.f;
	float TotalRecoveryX = 0.f, TotalRecoveryY = 0.f;
	float TotalRecoilX = 0.f, TotalRecoilY = 0.f;
	float TotalHorizontalRecoil = 0.f;

	/** current vertical recoil from continuous firing, used for smoothing pitch */
	float CurrentVerticalRecoil = 0.f;

	/** current vertical recoil from continuous firing, used for smoothing pitch */
	float CurrentHorizontalRecoil = 0.f;

	/** current spread from continuous firing */
	float CurrentFiringSpread = 0.f;

	/** Computes the horizontal recoil */
	float ComputeHorizontalRecoil();

	/** Computes the spread */
	FVector ComputeSpread(const FVector& ShootDir);

	/** Get play rate for reload animation */
	float GetReloadPlayRate(float AnimationLength);

	/** Start recovering from recoil*/
	void StartRecovering();

	/** play weapon animations */
	float PlayWeaponAnimation(UAnimMontage* Animation);

	/** stop playing weapon animations */
	void StopWeaponAnimation(const UAnimMontage& Animation);

	//////////////////////////////////////////////////////////////////////////
	// Input - server side

	UFUNCTION(reliable, server, WithValidation)
		void ServerStartFire();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStopFire();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerStartReload();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStopReload();

	UFUNCTION(reliable, server, WithValidation)
		void ServerAddAmmo();

	UFUNCTION(reliable, server, WithValidation)
		void ServerToggleAim(bool isAiming);

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
	/** Weapon Name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		FString Name;

	/** Weapon description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		FString Description;

	/** Damage dealt by the projectile */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
		uint8 NbProjectiles = 1;

	/** Damage dealt by the projectile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		int32 Damage = 20;

	/** Fire rate in rounds per minutes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		float FireRate = 600;
	
	/** Time between two consecutive shot while burst firing */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
		float TimeBetweenShotBurstFire = 0.05;

	/** Time between two consecutive shot while burst firing */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
		int32 NumberBurstShot = 3;

	/** Projectile velocity in meters per second */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		float ProjectileVelocity = 60.f;

	/** Projectile LifeSpan in seconds */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
		float ProjectileLifeSpan = 1.5f;

	/** Class of the weapon : auto, semi-auto, burst */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		WeaponClass WeaponClass = WeaponClass::WC_Auto;

	/** Class of the weapon : auto, semi-auto, burst */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		WeaponType WeaponType = WeaponType::WC_AR;
	
	/** Headshot damage multiplier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
		float HeadshotMultiplier = 2.f;


	//////////////////////////////////////////////////////////////////////////
	/** Reload time in seconds when there's still a bullet in the magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reload")
		float ShortReloadTime = 1.f;

	/** Reload time in seconds when there's no bullet in the magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reload")
		float LongReloadTime = 2.f;


	//////////////////////////////////////////////////////////////////////////
	/** Number of rounds in the magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magazine")
		int MagazineSize = 30;
	
	/** Amount of bullets left in the magazine */
	UPROPERTY(Transient, Replicated)
		int CurrentAmmoInClip = 0;

	/** Amount of bullet left in the reserve */
	UPROPERTY(Transient, Replicated)
		int CurrentAmmoInReserve = 0;
	
	/** Number of ammunition */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magazine")
		int MaxAmmo = 150;


	//////////////////////////////////////////////////////////////////////////
	/** Accuracy multiplier when aiming down the sight */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy")
		float AccuracyMultiplier = 2.f;

	/** Base weapon spread (degrees) */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Spread")
		float WeaponSpread = .5f;

	/** Continuous firing: spread increment */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Spread")
		float FiringSpreadIncrement = .25f;

	/** Continuous firing: max increment */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Spread")
		float FiringSpreadMax = 5.f;

	/** Vertical recoil in radians */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float VerticalRecoil = .5f;

	/** Horizontal recoil minimum in radians */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float HorizontalRecoilMin = .175f;
	
	/** Horizontal recoil maximum in radians */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float HorizontalRecoilMax = .175f;

	/** Horizontal recoil tolerance */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float HorizontalTolerance = .5f;

	/** Angle of recoil minimum  in degrees */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float AngleMin = 13.f;

	/** Angle or recoil maximum in degrees */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float AngleMax = 15.f;

	/** Recoil recovery delay in seconds */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float RecoilRecoveryDelay = .1f;

	/** Recoil recovery rate */
	UPROPERTY(EditDefaultsOnly, Category = "Accuracy|Recoil")
		float RecoilRecoveryRate = 1.f;


	//////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Transform)
		FTransform SpawnTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Transform)
		FTransform BackTransform;


	//////////////////////////////////////////////////////////////////////////
	// -===- Sound effect -===-
	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* FireSound;

	/** finished burst sound (bLoopedFireSound set) */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* FireFinishSound;

	/** out of ammo sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* OutOfAmmoSound;

	/** reload sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* ReloadSound;

	/** reload animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* ReloadAnim;

public :
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StartFiring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StopFiring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StartReloading(bool FromReplication = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StopReloading();

	/** trigger reload from server */
	UFUNCTION(Reliable, Client)
		void ClientStartReload();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool SetAim(bool isAiming);

	/** Fires a projectile. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void FireWeapon();

	/** add ammo to the reserve */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		bool AddAmmo();

};
