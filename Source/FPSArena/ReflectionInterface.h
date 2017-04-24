// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReflectionInterface.generated.h"



/**
 * 
 */
UCLASS()
class FPSARENA_API UReflectionInterface : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Reflection")
		static TArray<TSubclassOf<AAbstract_Weapon>> getAllWeaponClasses();

	UFUNCTION(BlueprintCallable, Category = "Reflection")
		static FString WeaponTypeToString(WeaponType wt);

	UFUNCTION(BlueprintCallable, Category = "Reflection")
		static FString WeaponClassToString(WeaponClass wc);
};
