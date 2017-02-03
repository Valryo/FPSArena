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

	CurrentState = EWeapon::Idle;
	WeaponClass = WeaponClass::WC_Auto;

	LastFireTime = 0.f;
}

void AAbstract_Weapon::SetWeaponState(EWeapon::State NewState)
{
	CurrentState = NewState;
}

EWeapon::State AAbstract_Weapon::GetCurrentState() const
{
	return EWeapon::State();
}

bool AAbstract_Weapon::CanFire() const
{
	return true;
}

bool AAbstract_Weapon::CanReload() const
{
	return false;
}

void AAbstract_Weapon::FireWeapon_Implementation()
{
	float timeBetweenShots = (1 / (FireRate / 60));

	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - LastFireTime > timeBetweenShots - 0.01)
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

		LastFireTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Failed to shoot - Time too short");
	}
}


bool AAbstract_Weapon::ToggleAim_Implementation()
{
	AimingDownSight = !AimingDownSight;

	return true;
}

bool AAbstract_Weapon::StartFiring_Implementation()
{
	if (CanFire())
	{
		CurrentState = EWeapon::Firing;
		float timeBetweenShots = (1 / (FireRate / 60));

		if (WeaponClass == WeaponClass::WC_Auto)
		{
			GetWorldTimerManager().SetTimer(RefireTimerHandle, this, &AAbstract_Weapon::FireWeapon_Implementation, timeBetweenShots, true, 0.f);
		}
		else if (WeaponClass == WeaponClass::WC_SemiAuto)
		{
			//GetWorldTimerManager().SetTimer(RefireTimerHandle, this, &AAbstract_Weapon::FireWeapon_Implementation, timeBetweenShots, false, 0.f);
			FireWeapon();
		}

		return true;
	}

	return false;
}

bool AAbstract_Weapon::StopFiring_Implementation()
{
	CurrentState = EWeapon::Idle;
	
	GetWorldTimerManager().ClearTimer(RefireTimerHandle);

	return true;
}
