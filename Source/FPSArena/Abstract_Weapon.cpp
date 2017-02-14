// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Weapon.h"
#include "Abstract_Projectile.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AAbstract_Weapon::AAbstract_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	AimingDownSight = false;
	PendingReload = false;
	IsEquipped = true;
	PendingEquip = false;
	WantsToFire = false;
	Refiring = false;

	CurrentState = EWeapon::Idle;
	WeaponClass = WeaponClass::WC_Auto;

	LastFireTime = 0.f;

	CurrentAmmoInClip = MagazineSize;
	CurrentAmmoLeft = MaxAmmo;

	ShortReloadTime = 0.f;
	LongReloadTime = 0.f;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
}

void AAbstract_Weapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoInClip = MagazineSize;
	CurrentAmmoLeft = MaxAmmo;

	TimeBetweenShots = 1.f / (FireRate / 60.f);
}

void AAbstract_Weapon::SetWeaponState(EWeapon::State NewState)
{
	const EWeapon::State PrevState = CurrentState;

	if (PrevState == EWeapon::Firing && NewState != EWeapon::Firing)
	{
		OnBurstFinished();
	}

	CurrentState = NewState;

	if (PrevState != EWeapon::Firing && NewState == EWeapon::Firing)
	{
		OnBurstStarted();
	}
}

EWeapon::State AAbstract_Weapon::GetCurrentState() const
{
	return EWeapon::State();
}

bool AAbstract_Weapon::CanFire() const
{
	float timeBetweenShots = (1 / (FireRate / 60));

	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - LastFireTime > timeBetweenShots - 0.01 && CurrentAmmoInClip > 0)
	{
		return true;
	}

	return false;
}

bool AAbstract_Weapon::CanReload() const
{
	bool GotAmmo = (CurrentAmmoInClip < MagazineSize) && (CurrentAmmoLeft > 0);
	bool StateOKToReload = ((CurrentState == EWeapon::Idle) || (CurrentState == EWeapon::Firing));

	return ((GotAmmo == true) && (StateOKToReload == true));
}

void AAbstract_Weapon::FireWeapon_Implementation()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FVector  ShootDir = GetCameraAim();
			FVector Origin = FP_MuzzleLocation->GetComponentLocation();

			ServerFireProjectile(Origin, ShootDir);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	
	//// try and play a firing animation if specified
	//if (FireAnimation != NULL)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != NULL)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}
}

bool AAbstract_Weapon::ServerFireProjectile_Validate(FVector Origin, FVector ShootDir)
{
	return true;
}

void AAbstract_Weapon::ServerFireProjectile_Implementation(FVector Origin, FVector ShootDir)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Server fire projectile");
	FTransform SpawnTM(ShootDir.Rotation(), Origin);
	AAbstract_Projectile* Projectile = Cast<AAbstract_Projectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM));
	
	if (Projectile)
	{
		Projectile->Instigator = Instigator;
		Projectile->SetOwner(this);
		Projectile->InitVelocity(ProjectileVelocity * 10);
		Projectile->InitProjectileProperties(Damage, ProjectileVelocity * 10, ProjectileLifeSpan);

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
	}
}

bool AAbstract_Weapon::ToggleAim_Implementation()
{
	AimingDownSight = !AimingDownSight;

	return true;
}

void AAbstract_Weapon::StartFiring_Implementation()
{
	if (Role < ROLE_Authority)
	{
		ServerStartFire();
	}

	if (!WantsToFire)
	{
		WantsToFire = true;
		DetermineWeaponState();
	}
}

void AAbstract_Weapon::StopFiring_Implementation()
{
	if (Role < ROLE_Authority)
	{
		ServerStopFire();
	}

	if (WantsToFire)
	{
		WantsToFire = false;
		DetermineWeaponState();
	}
}

bool AAbstract_Weapon::ServerStartFire_Validate()
{
	return true;
}

void AAbstract_Weapon::ServerStartFire_Implementation()
{
	StartFiring();
}

bool AAbstract_Weapon::ServerStopFire_Validate()
{
	return true;
}

void AAbstract_Weapon::ServerStopFire_Implementation()
{
	StopFiring();
}

void AAbstract_Weapon::StartReloading_Implementation()
{
	if (CanReload())
	{
		PendingReload = true;
		DetermineWeaponState();

		float reloadTime = (CurrentAmmoInClip > 0) ? ShortReloadTime : LongReloadTime;

		GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &AAbstract_Weapon::StopReloading, reloadTime, false);
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &AAbstract_Weapon::ReloadWeapon, reloadTime, false);
	}
}

void AAbstract_Weapon::StopReloading_Implementation()
{
	if (CurrentState == EWeapon::Reloading)
	{
		PendingReload = false;
		DetermineWeaponState();
	}
}


void AAbstract_Weapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(MagazineSize - CurrentAmmoInClip, CurrentAmmoLeft);

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta;
		CurrentAmmoLeft -= ClipDelta;
	}
}



void AAbstract_Weapon::UseAmmo()
{
	CurrentAmmoInClip--;
}

void AAbstract_Weapon::DetermineWeaponState()
{
	EWeapon::State NewState = EWeapon::Idle;

	if (IsEquipped)
	{
		if (PendingReload)
		{
			if (CanReload() == false)
			{
				NewState = CurrentState;
			}
			else
			{
				NewState = EWeapon::Reloading;
			}
		}
		else if ((PendingReload == false) && (WantsToFire == true) && (CanFire() == true))
		{
			NewState = EWeapon::Firing;
		}
	}
	else if (PendingEquip)
	{
		NewState = EWeapon::Equipping;
	}

	SetWeaponState(NewState);
}

void AAbstract_Weapon::OnBurstStarted()
{
	// start firing, can be delayed to satisfy TimeBetweenShots
	const float GameTime = GetWorld()->GetTimeSeconds();

	if (LastFireTime > 0 && TimeBetweenShots > 0.0f && LastFireTime + TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(RefireTimerHandle, this, &AAbstract_Weapon::HandleFiring, LastFireTime + TimeBetweenShots - GameTime, false);
	}
	else
	{
		HandleFiring();
	}
}

void AAbstract_Weapon::OnBurstFinished()
{
	GetWorldTimerManager().ClearTimer(RefireTimerHandle);
	Refiring = false;
}

void AAbstract_Weapon::HandleFiring()
{
	// local client will notify server
	if (Role == ROLE_SimulatedProxy)
	{
		ServerHandleFiring();
	}

	if (CurrentAmmoInClip > 0 && CanFire())
	{
		FireWeapon();
		UseAmmo();
	}

	// Out of ammo and still shooting
	if (CurrentAmmoLeft == 0 && !Refiring)
	{
		// Play a sound
	}

	// reload after firing last round
	if (CurrentAmmoInClip <= 0 && CanReload())
	{
		StartReloading();
	}

	// setup refire timer
	Refiring = (CurrentState == EWeapon::Firing && WeaponClass == WeaponClass::WC_Auto);

	if (Refiring)
	{
		GetWorldTimerManager().SetTimer(RefireTimerHandle, this, &AAbstract_Weapon::HandleFiring, TimeBetweenShots, false);
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

bool AAbstract_Weapon::ServerHandleFiring_Validate()
{
	return true;
}

void AAbstract_Weapon::ServerHandleFiring_Implementation()
{
	const bool bShouldUpdateAmmo = (CurrentAmmoInClip > 0 && CanFire());

	HandleFiring();

	if (bShouldUpdateAmmo)
	{
		// update ammo
		UseAmmo();
	}
}

FVector AAbstract_Weapon::GetCameraAim() const
{
	APlayerController* const PlayerController = Instigator ? Cast<APlayerController>(Instigator->Controller) : NULL;
	FVector FinalAim = FVector::ZeroVector;

	if (PlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if (Instigator)
	{
		FinalAim = Instigator->GetBaseAimRotation().Vector();
	}

	return FinalAim;
}

void AAbstract_Weapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 
    // Replicate to everyone
    DOREPLIFETIME(AAbstract_Weapon, CurrentAmmoInClip);
	DOREPLIFETIME(AAbstract_Weapon, CurrentAmmoLeft);
	
}
