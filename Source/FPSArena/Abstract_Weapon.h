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

	/** spawn projectile on server */
	UFUNCTION(reliable, server, WithValidation)
		void ServerFireProjectile(FVector Origin, FVector ShootDir);


	FVector GetCameraAim() const;


	//////////////////////////////////////////////////////////////////////////
	// Input - server side

	UFUNCTION(reliable, server, WithValidation)
		void ServerStartFire();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStopFire();

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
	
	/** amount of bullets left in the magazine */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
	int CurrentAmmoInClip;

	/** amount of bullet left in the reserve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
	int CurrentAmmoLeft;
	
	/** Number of ammunition */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
		int MaxAmmo;

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

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* FireSound;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* EmptyMagSound;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* ReloadSound;

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
