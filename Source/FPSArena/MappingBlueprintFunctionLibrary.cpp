// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "MappingBlueprintFunctionLibrary.h"

TArray<FInputActionKeyMapping> UMappingBlueprintFunctionLibrary::getActions() {
	
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputActionKeyMapping> map = const_cast<UInputSettings*>(Settings)->ActionMappings;
	
	return map;
}

TArray<FInputAxisKeyMapping> UMappingBlueprintFunctionLibrary::getAxis() {

	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputAxisKeyMapping> map = const_cast<UInputSettings*>(Settings)->AxisMappings;

	return map;
}

FString UMappingBlueprintFunctionLibrary::ActionToString(FInputActionKeyMapping Action) {

	FString name = "";
	Action.ActionName.AppendString(name);
	
	return name;
}

FString UMappingBlueprintFunctionLibrary::AxisToString(FInputAxisKeyMapping Axis) {

	FString name = "";
	Axis.AxisName.AppendString(name);
	if (name.Contains("Forward") && Axis.Scale < 0) {
		name = "MoveBackward";
	}
	if (name.Contains("Right") && Axis.Scale < 0) {
		name = "MoveLeft";
	}

	return name;
}

FKey UMappingBlueprintFunctionLibrary::getActionKey(FInputActionKeyMapping Action) {
	
	FKey key = Action.Key;
	
	return key;
}

FKey UMappingBlueprintFunctionLibrary::getAxisKey(FInputAxisKeyMapping Axis) {

	FKey key = Axis.Key;

	return key;
}

FText UMappingBlueprintFunctionLibrary::KeyToText(FKey Key) {

	FText name = Key.GetDisplayName();

	return name;
}

FInputActionKeyMapping UMappingBlueprintFunctionLibrary::setAction(FInputActionKeyMapping Action, FKey Key) {

	Action.Key = Key;

	return Action;
}

FInputAxisKeyMapping UMappingBlueprintFunctionLibrary::setAxis(FInputAxisKeyMapping Axis, FKey Key) {

	Axis.Key = Key;
	
	return Axis;
}

FString UMappingBlueprintFunctionLibrary::saveMapping(TArray<FInputActionKeyMapping> Actions, TArray<FInputAxisKeyMapping> Axis) {

	FString show = "";

	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	
	// Actions Update
	TArray<FInputActionKeyMapping> oldActions = const_cast<UInputSettings*>(Settings)->ActionMappings;
	for (int32 Index = 0; Index < oldActions.Num(); ++Index) {
		const_cast<UInputSettings*>(Settings)->RemoveActionMapping(oldActions[Index]);
	}
	for (int32 Index = 0; Index < Actions.Num(); ++Index) {
		const_cast<UInputSettings*>(Settings)->AddActionMapping(Actions[Index]);
		Actions[Index].ActionName.AppendString(show);
		show.Append("\n");
		Actions[Index].Key.GetFName().AppendString(show);
		show.Append("\n\n");
	}

	// Axis Update
	TArray<FInputAxisKeyMapping> oldAxis = const_cast<UInputSettings*>(Settings)->AxisMappings;
	for (int32 Index = 0; Index < oldAxis.Num(); ++Index) {
		const_cast<UInputSettings*>(Settings)->RemoveAxisMapping(oldAxis[Index]);
	}
	for (int32 Index = 0; Index < Axis.Num(); ++Index) {
		const_cast<UInputSettings*>(Settings)->AddAxisMapping(Axis[Index]);
	}

	//SAVES TO DISK
	const_cast<UInputSettings*>(Settings)->SaveKeyMappings();
	const_cast<UInputSettings*>(Settings)->SaveConfig(16384Ui64, L"../../../FPSArena/Config/DefaultInput.ini");

	for (TObjectIterator<UPlayerInput> It; It; ++It)
	{
		It->ForceRebuildingKeyMaps(true);
	}

	return show;
}