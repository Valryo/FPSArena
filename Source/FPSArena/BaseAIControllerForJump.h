// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIControllerForJump.generated.h"

/**
*
*/
UCLASS(CustomConstructor)
class FPSARENA_API ABaseAIControllerForJump : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIControllerForJump(const FObjectInitializer& ObjectInitializer);

};