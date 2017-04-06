// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "ReflectionInterface.h"
#include "Abstract_Weapon.h"

TArray<TSubclassOf<AAbstract_Weapon>> UReflectionInterface::getAllWeaponClasses()
{
	TArray<TSubclassOf<AAbstract_Weapon>> WeaponsClasses;
	
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(AAbstract_Weapon::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
		{
			WeaponsClasses.Add(*It);
		}
	}

	return WeaponsClasses;
}
