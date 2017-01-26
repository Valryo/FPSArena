// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "Abstract_Weapon.h"


// Sets default values
AAbstract_Weapon::AAbstract_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AAbstract_Weapon::Fire()
{
	return false;
}

bool AAbstract_Weapon::Reload()
{
	return false;
}

bool AAbstract_Weapon::Aim()
{
	return false;
}
