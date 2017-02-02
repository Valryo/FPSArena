// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Weapon.h"


// Sets default values
AAbstract_Weapon::AAbstract_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

void AAbstract_Weapon::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

bool AAbstract_Weapon::ToggleAim_Implementation()
{
	//AimingDownSight = !AimingDownSight;

	return true;
}

bool AAbstract_Weapon::StartFiring_Implementation()
{
	/*if (CurrentState == EWeaponState::Idle)
	{
		this->CurrentState = EWeaponState::Firing;

		return true;
	}*/

	return false;
}

bool AAbstract_Weapon::StopFiring_Implementation()
{
	/*if (CurrentState == EWeaponState::Firing)
	{
		this->CurrentState = EWeaponState::Idle;

		return true;
	}*/
	
	return false;
}

void AAbstract_Weapon::Tick(float DeltaTime)
{
	//if (CurrentState == EWeaponState::Firing)
	//{
	//	//FVector ShootDir = GetAdjustedAim();
	//	//FVector Origin = GetMuzzleLocation();
	//}
}
