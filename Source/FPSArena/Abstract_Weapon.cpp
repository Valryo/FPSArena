// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Weapon.h"
#include "Abstract_Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAbstract_Weapon::AAbstract_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);
	FP_Gun->bCastDynamicShadow = true;
	FP_Gun->CastShadow = true;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//RootComponent = FP_Gun;
	FP_Gun->SetupAttachment(RootComponent);

	FP_SightSocket = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sight"));
	FP_Gun->SetOnlyOwnerSee(false);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_SightSocket->SetupAttachment(FP_Gun);

	AimingDownSight = false;
	PendingReload = false;
	IsEquipped = true;
	PendingEquip = false;
	WantsToFire = false;
	Refiring = false;
	Bursting = false;
	Fired = false;
	Recovering = false;

	CurrentState = EWeapon::Idle;
	WeaponClass = WeaponClass::WC_Auto;

	LastFireTime = 0.f;

	CurrentAmmoInClip = MagazineSize;
	CurrentAmmoInReserve = MaxAmmo;
	BurstCounter = 0;

	ShortReloadTime = 0.f;
	LongReloadTime = 0.f;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

	WeaponSpread = 5.f;
	FiringSpreadIncrement = 1.f;
	FiringSpreadMax = 10.f;
}

void AAbstract_Weapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	CurrentFiringSpread = WeaponSpread;
}

void AAbstract_Weapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoInClip = MagazineSize;
	CurrentAmmoInReserve = MaxAmmo;

	TimeBetweenShots = 1.f / (FireRate / 60.f);
}

void AAbstract_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	APlayerController* PC = Instigator ? Cast<APlayerController>(Instigator->Controller) : NULL;
	float X = 0.f, Y = 0.f;

	if (CurrentState == EWeapon::Firing)
	{
		if (PC)
		{
			PC->GetInputMouseDelta(X, Y);
		}

		if (Fired)
		{
			CurrentVerticalRecoil = 0.f;
			CurrentHorizontalRecoil = 0.f;

			TotalRecoilX = 0.f;
			TotalRecoilY = 0.f;

			Fired = false;
		}

		CurrentVerticalRecoil = FMath::FInterpTo(CurrentVerticalRecoil, GetImprovedAccuracy(VerticalRecoil), DeltaTime, 10.0f);
		CurrentHorizontalRecoil = FMath::FInterpTo(CurrentHorizontalRecoil, TotalHorizontalRecoil, DeltaTime, 10.0f);

		float recoilY = (TotalRecoilY + CurrentVerticalRecoil > VerticalRecoil) ? VerticalRecoil - TotalRecoilY : CurrentVerticalRecoil;
		float recoilX = (TotalRecoilX + CurrentHorizontalRecoil > TotalHorizontalRecoil) ? TotalHorizontalRecoil - TotalRecoilX : CurrentHorizontalRecoil;

		if (TotalRecoilX < TotalHorizontalRecoil)
		{
			MyPawn->AddControllerYawInput(recoilX);
			RecoveryX += recoilX;

			if (X != 0 && UKismetMathLibrary::SignOfFloat(X) != UKismetMathLibrary::SignOfFloat(recoilX))
			{
				RecoveryX -= FMath::Min(RecoveryX, RecoveryX + X);
			}
		}

		if (TotalRecoilY < VerticalRecoil)
		{
			MyPawn->AddControllerPitchInput(-recoilY);
			RecoveryY += recoilY;


			if (Y < 0)
			{
				RecoveryY -= FMath::Min(RecoveryY, RecoveryY + Y);
			}
		}

		TotalRecoilX += CurrentHorizontalRecoil; 
		TotalRecoilY += CurrentVerticalRecoil;
	}

	if (Recovering)
	{
		CurrentRecoveryX = FMath::FInterpTo(CurrentRecoveryX, RecoveryX, DeltaTime, RecoilRecoveryRate);
		CurrentRecoveryY = FMath::FInterpTo(CurrentRecoveryY, RecoveryY, DeltaTime, RecoilRecoveryRate);

		float recoveryY = (TotalRecoveryY + CurrentRecoveryY > RecoveryY) ? RecoveryY - TotalRecoveryY : CurrentRecoveryY; 
		float recoveryX = (TotalRecoveryX + CurrentRecoveryX > RecoveryX) ? RecoveryX - TotalRecoveryX : CurrentRecoveryX;

		if (TotalRecoveryX < RecoveryX)
		{
			MyPawn->AddControllerYawInput(-recoveryX);
		}
		
		if (TotalRecoveryY < RecoveryY)
		{
			MyPawn->AddControllerPitchInput(recoveryY);
		}
		
		TotalRecoveryY += CurrentRecoveryY;
		TotalRecoveryX += CurrentRecoveryX;
		
		if (TotalRecoveryY > RecoveryY && TotalRecoveryX > RecoveryX)
		{
			Recovering = false;
			CurrentRecoveryY = 0.f;
			TotalRecoveryY = 0.f;

			CurrentRecoveryX = 0.f;
			TotalRecoveryX = 0.f;
		}
	}
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

bool AAbstract_Weapon::CanFire_Implementation() const
{
	bool bStateOKToFire = ((CurrentState == EWeapon::Idle) || (CurrentState == EWeapon::Firing));
	return ((bStateOKToFire == true) && (PendingReload == false));
}

bool AAbstract_Weapon::CanReload() const
{
	bool GotAmmo = (CurrentAmmoInClip < MagazineSize) && (CurrentAmmoInReserve > 0);
	bool StateOKToReload = ((CurrentState == EWeapon::Idle) || (CurrentState == EWeapon::Firing));

	return ((GotAmmo == true) && (StateOKToReload == true) && !AimingDownSight);
}

FVector AAbstract_Weapon::GetCameraDamageStartLocation(const FVector& AimDir) const
{
	APlayerController* PC = Instigator ? Cast<APlayerController>(Instigator->Controller) : NULL;
	
	FVector OutStartTrace = FVector::ZeroVector;

	if (PC)
	{
		// use player's camera
		FRotator UnusedRot;
		PC->GetPlayerViewPoint(OutStartTrace, UnusedRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutStartTrace = OutStartTrace + AimDir * ((Instigator->GetActorLocation() - OutStartTrace) | AimDir);
	}

	return OutStartTrace;
}

FHitResult AAbstract_Weapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Pawn, TraceParams);

	return Hit;
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
			
			FVector Origin = FP_Gun->GetSocketLocation(MuzzleAttachPoint);

			// trace from camera to check what's under crosshair
			const float ProjectileAdjustRange = 10000.0f;
			const FVector StartTrace = GetCameraDamageStartLocation(ShootDir);
			const FVector EndTrace = StartTrace + ShootDir * ProjectileAdjustRange;
			FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

			// and adjust directions to hit that actor
			if (Impact.bBlockingHit)
			{
				const FVector AdjustedDir = (Impact.ImpactPoint - Origin).GetSafeNormal();
				bool bWeaponPenetration = false;

				const float DirectionDot = FVector::DotProduct(AdjustedDir, ShootDir);
				if (DirectionDot < 0.0f)
				{
					// shooting backwards = weapon is penetrating
					bWeaponPenetration = true;
				}
				else if (DirectionDot < 0.5f)
				{
					// check for weapon penetration if angle difference is big enough
					// raycast along weapon mesh to check if there's blocking hit
					FVector MuzzleStartTrace = Origin - FP_Gun->GetSocketRotation(MuzzleAttachPoint).Vector() * 150.0f;
					FVector MuzzleEndTrace = Origin;
					FHitResult MuzzleImpact = WeaponTrace(MuzzleStartTrace, MuzzleEndTrace);

					if (MuzzleImpact.bBlockingHit)
					{
						bWeaponPenetration = true;
					}
				}

				if (bWeaponPenetration)
				{
					// spawn at crosshair position
					Origin = Impact.ImpactPoint - ShootDir * 10.0f;
				}
				else
				{
					// adjust direction to hit
					ShootDir = AdjustedDir;
				}
			}

			if (BurstCounter == NumberBurstShot && WeaponClass == WeaponClass::WC_Burst)
			{
				Bursting = false;
				StopFiring();
			}

			// Spread
			FVector AimDir = ComputeSpread(ShootDir);

			// Recoil
			TotalHorizontalRecoil = ComputeHorizontalRecoil();

			// Spawn projectile on the server
			ServerFireProjectile(Origin, AimDir);
			Fired = true;
		}
	}
}

FVector AAbstract_Weapon::ComputeSpread(const FVector& ShootDir)
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentFiringSpread * 0.5f);

	const FVector AimDir = WeaponRandomStream.VRandCone(ShootDir, ConeHalfAngle, ConeHalfAngle);
	CurrentFiringSpread = FMath::Min(FiringSpreadMax, CurrentFiringSpread + GetImprovedAccuracy(FiringSpreadIncrement));

	return AimDir;
}

float AAbstract_Weapon::ComputeHorizontalRecoil()
{
	float FinalRecoilYaw = FMath::FRandRange(GetImprovedAccuracy(HorizontalRecoilMin), GetImprovedAccuracy(HorizontalRecoilMax));
	float RecoilAngle = FMath::FRandRange(AngleMin, AngleMax);

	if (FGenericPlatformMath::Abs(HorizontalRecoil) < HorizontalTolerance)
	{
		FinalRecoilYaw *= FMath::RoundFromZero(FMath::FRandRange(-1, 1));
	}
	else if (HorizontalRecoil > 0)
	{
		FinalRecoilYaw *= -1;
	}

	HorizontalRecoil += FinalRecoilYaw;

	return FinalRecoilYaw + FMath::DegreesToRadians(RecoilAngle);
}

bool AAbstract_Weapon::ServerFireProjectile_Validate(FVector Origin, FVector ShootDir)
{
	return true;
}

void AAbstract_Weapon::ServerFireProjectile_Implementation(FVector Origin, FVector ShootDir)
{
	FTransform SpawnTM(ShootDir.Rotation(), Origin);
	AAbstract_Projectile* Projectile = Cast<AAbstract_Projectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM));
	
	if (Projectile)
	{
		Projectile->Instigator = Instigator;
		Projectile->SetOwner(this);
		Projectile->InitVelocity(ProjectileVelocity * 10);
		Projectile->InitProjectileProperties(Damage, HeadshotMultiplier, ProjectileVelocity * 100, ProjectileLifeSpan);
		Projectile->SetOrigin(Origin);

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
	if (!Bursting)
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
	if (Role < ROLE_Authority)
	{
		ServerStartReload();
	}

	if (CanReload())
	{
		APawn* MyPawn = Cast<APawn>(GetOwner());

		PendingReload = true;
		DetermineWeaponState();

		float reloadTime = GetReloadDuration();

		GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &AAbstract_Weapon::StopReloading, reloadTime, false);

		if (Role == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &AAbstract_Weapon::ReloadWeapon, reloadTime, false);
		}

		if (MyPawn->IsLocallyControlled())
		{
			PlayWeaponSound(ReloadSound);
		}

		PlayWeaponAnimation(ReloadAnim);
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

bool AAbstract_Weapon::ServerStartReload_Validate()
{
	return true;
}

void AAbstract_Weapon::ServerStartReload_Implementation()
{
	StartReloading();
}

bool AAbstract_Weapon::ServerStopReload_Validate()
{
	return true;
}

void AAbstract_Weapon::ServerStopReload_Implementation()
{
	StopReloading();
}

bool AAbstract_Weapon::ServerAddAmmo_Validate()
{
	return true;
}

void AAbstract_Weapon::ServerAddAmmo_Implementation()
{
	AddAmmo();
}

void AAbstract_Weapon::StartRecovering()
{
	Recovering = true;
}

void AAbstract_Weapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(MagazineSize - CurrentAmmoInClip, CurrentAmmoInReserve);

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta;
		CurrentAmmoInReserve -= ClipDelta;
	}
}

void AAbstract_Weapon::UseAmmo()
{
	CurrentAmmoInClip--;
}

void AAbstract_Weapon::AddAmmo_Implementation()
{
	if (Role < ROLE_Authority)
	{
		ServerAddAmmo();
	}

	if (CurrentAmmoInReserve < MaxAmmo)
	{
		CurrentAmmoInReserve += FMath::Min(MagazineSize, MaxAmmo - CurrentAmmoInReserve);
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
	InitialRotation = GetCameraAim();
	RecoveryX = 0.f;
	RecoveryY = 0.f;

	Recovering = false;

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
	BurstCounter = 0;
	CurrentFiringSpread = WeaponSpread;
	HorizontalRecoil = 0.f;
	TotalHorizontalRecoil = 0.f;

	GetWorldTimerManager().SetTimer(TimerHandle_StartRecover, this, &AAbstract_Weapon::StartRecovering, RecoilRecoveryDelay, false);

	// stop firing FX locally, unless it's a dedicated server
	if (GetNetMode() != NM_DedicatedServer)
	{
		StopSimulatingWeaponFire();
	}
}

void AAbstract_Weapon::HandleFiring()
{
	APawn* MyPawn = Cast<APawn>(GetOwner());

	if (CurrentAmmoInClip > 0 && CanFire())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			SimulateWeaponFire();
		}

		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			FireWeapon();
			UseAmmo();

			BurstCounter++;
		}
	}
	else if (CanReload())
	{
		StartReloading();
	}
	else if (MyPawn && MyPawn->IsLocallyControlled())
	{
		if (CurrentAmmoInClip == 0  && CurrentAmmoInReserve == 0 && !Refiring)
		{
			// Play out of ammo sound
			PlayWeaponSound(OutOfAmmoSound);
		}

		// stop weapon fire FX, but stay in Firing state
		if (BurstCounter > 0)
		{
			OnBurstFinished();
		}
	}

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		// local client will notify server
		if (Role < ROLE_Authority)
		{
			ServerHandleFiring();
		}
		
		// reload after firing last round
		if (CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReloading();
		}

		float RefireTime = 0;
		switch (WeaponClass)
		{
		case WeaponClass::WC_Auto:
			Refiring = CurrentState == EWeapon::Firing;
			RefireTime = TimeBetweenShots;
			break;
		case WeaponClass::WC_Burst:
			Refiring = CurrentState == EWeapon::Firing && BurstCounter < NumberBurstShot;
			RefireTime = TimeBetweenShotBurstFire;
			Bursting = true;
			break;
		case WeaponClass::WC_SemiAuto:
			Refiring = false;
			break;
		}

		if (Refiring)
		{
			GetWorldTimerManager().SetTimer(RefireTimerHandle, this, &AAbstract_Weapon::HandleFiring, RefireTime, false);
		}
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
	DOREPLIFETIME(AAbstract_Weapon, CurrentAmmoInReserve);
	
	DOREPLIFETIME_CONDITION(AAbstract_Weapon, BurstCounter, COND_SkipOwner);
}

UAudioComponent* AAbstract_Weapon::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	APawn* MyPawn = Cast<APawn>(GetOwner());

	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}

float AAbstract_Weapon::GetReloadDuration()
{
	return (CurrentAmmoInClip > 0) ? ShortReloadTime : LongReloadTime;
}

void AAbstract_Weapon::SimulateWeaponFire()
{
	if (Role == ROLE_Authority && CurrentState != EWeapon::Firing)
	{
		return;
	}

	if (MuzzleFX)
	{
		if (MuzzlePSC == NULL)
		{
			//FP_Gun->GetSocketLocation(MuzzleAttachPoint);
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FP_Gun->GetSocketLocation(MuzzleAttachPoint).ToString());

			//MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FP_Gun, MuzzleAttachPoint);
			MuzzlePSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFX, FP_Gun->GetSocketTransform(MuzzleAttachPoint));

			//// Split screen requires we create 2 effects. One that we see and one that the other player sees.
			//if ((MyPawn != NULL) && (MyPawn->IsLocallyControlled() == true))
			//{
			//	AController* PlayerCon = MyPawn->GetController();
			//	if (PlayerCon != NULL)
			//	{
			//		Mesh1P->GetSocketLocation(MuzzleAttachPoint);
			//		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh1P, MuzzleAttachPoint);
			//		MuzzlePSC->bOwnerNoSee = false;
			//		MuzzlePSC->bOnlyOwnerSee = true;

			//		Mesh3P->GetSocketLocation(MuzzleAttachPoint);
			//		MuzzlePSCSecondary = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh3P, MuzzleAttachPoint);
			//		MuzzlePSCSecondary->bOwnerNoSee = true;
			//		MuzzlePSCSecondary->bOnlyOwnerSee = false;
			//	}
			//}
			//else
			//{
			//	MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, UseWeaponMesh, MuzzleAttachPoint);
			//}
		}
	}

	//if (!bLoopedFireAnim || !bPlayingFireAnim)
	//{
	//	PlayWeaponAnimation(FireAnim);
	//	bPlayingFireAnim = true;
	//}
	
	if (LoopedFireSound)
	{
		if (FireAC == NULL)
		{
			FireAC = PlayWeaponSound(FireLoopSound);
		}
	}
	else
	{
		PlayWeaponSound(FireSound);
	}
}

void AAbstract_Weapon::StopSimulatingWeaponFire()
{
	if (MuzzlePSC != NULL)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = NULL;
	}
	//if (bLoopedMuzzleFX)
	//{
	//	if (MuzzlePSC != NULL)
	//	{
	//		MuzzlePSC->DeactivateSystem();
	//		MuzzlePSC = NULL;
	//	}
	//	if (MuzzlePSCSecondary != NULL)
	//	{
	//		MuzzlePSCSecondary->DeactivateSystem();
	//		MuzzlePSCSecondary = NULL;
	//	}
	//}

	//if (bLoopedFireAnim && bPlayingFireAnim)
	//{
	//	StopWeaponAnimation(FireAnim);
	//	bPlayingFireAnim = false;
	//}

	if (FireAC)
	{
		FireAC->FadeOut(0.1f, 0.0f);
		FireAC = NULL;

		PlayWeaponSound(FireFinishSound);
	}
}

float AAbstract_Weapon::GetReloadPlayRate(float AnimationLength)
{
	return AnimationLength / GetReloadDuration();
}

float AAbstract_Weapon::PlayWeaponAnimation(UAnimMontage* Animation)
{
	ACharacter* Character = Instigator ? Cast<ACharacter>(Instigator) : nullptr;
	float Duration = 0.0f;

	if (Character)
	{
		if (Animation)
		{
			Duration = Animation->GetPlayLength() / GetReloadDuration();
			Character->PlayAnimMontage(Animation, Duration);
		}
	}

	return Duration;
}

void AAbstract_Weapon::StopWeaponAnimation(const UAnimMontage& Animation)
{
	/*if (MyPawn)
	{
		UAnimMontage* UseAnim = MyPawn->IsFirstPerson() ? Animation.Pawn1P : Animation.Pawn3P;
		if (UseAnim)
		{
			MyPawn->StopAnimMontage(UseAnim);
		}
	}*/
}

float AAbstract_Weapon::GetImprovedAccuracy(float f)
{
	return (AimingDownSight ? f / AccuracyMultiplier : f);
}

void AAbstract_Weapon::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		SimulateWeaponFire();
	}
	else
	{
		StopSimulatingWeaponFire();
	}
}