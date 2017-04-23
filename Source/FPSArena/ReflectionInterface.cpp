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
	AssetRegistry.GetAssetsByPath(FName("/Game/Weapon/FPWeapon/Blueprint"), AssetList, /*bRecursive=*/true);
	
	auto Library = UObjectLibrary::CreateLibrary(AAbstract_Weapon::StaticClass(), true, GIsEditor);
	Library->LoadBlueprintAssetDataFromPath("/Game/Weapon/FPWeapon/Blueprint");

	TArray<FAssetData> Assets;
	Library->GetAssetDataList(Assets);

	// Ensure all assets are loaded and store their class
	TArray<TSubclassOf<AAbstract_Weapon>> EventClasses;
	for (const FAssetData& Asset : Assets)
	{
		//if (Asset.AssetName.ToString().StartsWith("BP_Weapon_"))
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, Asset.AssetName.ToString());
		//	Asset.GetAsset();
		//	//EventClasses.Add(Asset.GetClass());
		//	
		//}

		const UBlueprint* BlueprintObj = Cast<UBlueprint>(Asset.GetAsset());
		if (BlueprintObj)
		{
			UClass* BlueprintClass = BlueprintObj->GeneratedClass;
			if (BlueprintClass)
			{
				EventClasses.Add(BlueprintClass);
			}
		}
		else
		{
			FString GeneratedClassName = Asset.AssetName.ToString().Append("_C");

			UClass* Class = FindObject<UClass>(Asset.GetPackage(), *GeneratedClassName);
			if (Class)
			{
				EventClasses.Add(Class);
			}
			else
			{
				UObjectRedirector* RenamedClassRedirector = FindObject<UObjectRedirector>(Asset.GetPackage(), *GeneratedClassName);
				if (RenamedClassRedirector)
				{
					EventClasses.Add(CastChecked<UClass>(RenamedClassRedirector->DestinationObject));
				}
			}
		}

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
		
	return EventClasses;
}

FString UReflectionInterface::WeaponTypeToString(WeaponType wt)
{
	switch (wt)
	{
	case WeaponType::WC_LMG:
		return "LMG";
	case WeaponType::WC_SMG:
		return "SMG";
	case WeaponType::WC_Carbine:
		return "Carbine";
	case WeaponType::WC_AR:
		return "Assault Rifle";
	case WeaponType::WC_Shotgun:
		return "Shotgun";
	}

	return "";
}

FString UReflectionInterface::WeaponClassToString(WeaponClass wc)
{
	switch (wc)
	{
	case WeaponClass::WC_Auto:
		return "Automatic";
	case WeaponClass::WC_SemiAuto:
		return "Semi-Automatic";
	case WeaponClass::WC_Burst:
		return "Burst firing";
	}

	return "";
}