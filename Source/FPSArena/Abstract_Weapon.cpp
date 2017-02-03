// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Weapon.h"
#include "Abstract_Projectile.h"


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
	bool GotAmmo = (CurrentAmmoInClip < MagazineSize) && (CurrentAmmoLeft - CurrentAmmoInClip > 0);
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
			FRotator SpawnRotation = GetActorRotation();
			SpawnRotation.Yaw += 90;
			const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AAbstract_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, SpawnLocation.ToString() + "\n" + SpawnRotation.ToString());
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(CurrentAmmoInClip) + " - " + FString::FromInt(CurrentAmmoLeft));
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

bool AAbstract_Weapon::ToggleAim_Implementation()
{
	AimingDownSight = !AimingDownSight;

	return true;
}

void AAbstract_Weapon::StartFiring_Implementation()
{
	if (!WantsToFire)
	{
		WantsToFire = true;
		DetermineWeaponState();
	}
}

void AAbstract_Weapon::StopFiring_Implementation()
{
	if (WantsToFire)
	{
		WantsToFire = false;
		DetermineWeaponState();
	}
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Reloaded");
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

	if (CurrentAmmoInClip <= 0)
	{
		StopFiring();
		
		if (CanReload())
		{

		}
	}
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