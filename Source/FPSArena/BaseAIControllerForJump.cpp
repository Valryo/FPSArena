// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "PathFollowingComponentForJump.h"
#include "BaseAIControllerForJump.h"


ABaseAIControllerForJump::ABaseAIControllerForJump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPathFollowingComponentForJump>(TEXT("PathFollowingComponent")))
{
bWantsPlayerState = true;

}


