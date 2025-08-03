// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "CoreMinimal.h"
#include "DreamWidgetTweenDelegateHandleWrapper.generated.h"

/** Just a wrapper for blueprint to store a delegate handle */
USTRUCT(BlueprintType)
struct DREAMANIMATIONTEXTBLOCK_API FDreamWidgetTweenDelegateHandleWrapper
{
	GENERATED_BODY()
public:
	FDreamWidgetTweenDelegateHandleWrapper() {}
	FDreamWidgetTweenDelegateHandleWrapper(FDelegateHandle InDelegateHandle)
	{
		DelegateHandle = InDelegateHandle;
	}
	FDelegateHandle DelegateHandle;
};
