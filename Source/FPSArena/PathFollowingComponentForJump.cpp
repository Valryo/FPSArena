// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSArena.h"
#include "NavArea_Jump.h"
#include "PathFollowingComponentForJump.h"


void UPathFollowingComponentForJump::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (CharacterMoveComp != NULL)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

		//GetPathPoint(Path, MoveSegmentStartIndex, SegmentStart)

		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			// jump! well... fly-in-straight-line!
			CharacterMoveComp->SetMovementMode(MOVE_Flying);
		}
		else
		{
			// regular move
			CharacterMoveComp->SetMovementMode(MOVE_Walking);
		}
	}
}
void UPathFollowingComponentForJump::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}


