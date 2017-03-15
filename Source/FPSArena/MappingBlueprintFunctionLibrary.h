<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MappingBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FPSARENA_API UMappingBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mapping")
		static TArray<FInputActionKeyMapping> getActions();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mapping")
		static TArray<FInputAxisKeyMapping> getAxis();

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FString ActionToString(FInputActionKeyMapping Action);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FString AxisToString(FInputAxisKeyMapping Axis);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FKey getActionKey(FInputActionKeyMapping Action);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FKey getAxisKey(FInputAxisKeyMapping Axis);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FText KeyToText(FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FInputActionKeyMapping setAction(FInputActionKeyMapping Action, FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FInputAxisKeyMapping setAxis(FInputAxisKeyMapping Axis, FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FString saveMapping(TArray<FInputActionKeyMapping> Actions, TArray<FInputAxisKeyMapping> Axis);

private:

};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MappingBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FPSARENA_API UMappingBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mapping")
		static TArray<FInputActionKeyMapping> getActions();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mapping")
		static TArray<FInputAxisKeyMapping> getAxis();

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FString ActionToString(FInputActionKeyMapping Action);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FString AxisToString(FInputAxisKeyMapping Axis);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FKey getActionKey(FInputActionKeyMapping Action);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FKey getAxisKey(FInputAxisKeyMapping Axis);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FText KeyToText(FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FInputActionKeyMapping setAction(FInputActionKeyMapping Action, FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FInputAxisKeyMapping setAxis(FInputAxisKeyMapping Axis, FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Mapping")
		static FString saveMapping(TArray<FInputActionKeyMapping> Actions, TArray<FInputAxisKeyMapping> Axis);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Display")
		static TArray<FString> getSupportedResolutionsString();

private:
};
>>>>>>> refs/remotes/origin/Nicow
