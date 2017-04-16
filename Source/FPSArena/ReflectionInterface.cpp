// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "ReflectionInterface.h"
#include "AssetRegistryModule.h"
#include "ARFilter.h"
#include "Abstract_Weapon.h"

TArray<TSubclassOf<AAbstract_Weapon>> UReflectionInterface::getAllWeaponClasses()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Get all assets in the path, does not load them
	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssetsByPath(FName("/Game/Weapon/FPWeapon/"), AssetList, /*bRecursive=*/true);
	
	// Ensure all assets are loaded and store their class
	TArray<TSubclassOf<AAbstract_Weapon>> EventClasses;
	for (const FAssetData& Asset : AssetList)
	{
		Asset.GetAsset();
		//
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, Asset.AssetName.ToString());
		//// Skip non blueprint assets
		//const UBlueprint* BlueprintObj = Cast<UBlueprint>(Asset.GetAsset());
		//if (!BlueprintObj)
		//	continue;
		//
		//// Check whether blueprint class has parent class we're looking for
		//UClass* BlueprintClass = BlueprintObj->GeneratedClass;
		//if (!BlueprintClass || !BlueprintClass->IsChildOf(AAbstract_Weapon::StaticClass()))
		//	continue;

		//// Store class
		//EventClasses.Add(BlueprintClass);
	}

	for (TObjectIterator<UClass> Itr; Itr; ++Itr)
	{
		if (Itr->IsChildOf(AAbstract_Weapon::StaticClass()) && !Itr->HasAnyClassFlags(CLASS_Abstract))
		{
			EventClasses.Add(*Itr);
		}
	}
		
	return EventClasses;
}
