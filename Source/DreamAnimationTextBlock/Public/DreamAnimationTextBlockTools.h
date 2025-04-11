// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

namespace FDreamAnimationTextBlockTools
{
	struct DREAMANIMATIONTEXTBLOCK_API FDelayStateBase
	{
		TWeakObjectPtr<UObject> WorldContextObject;
		float Delay = 0.f;
		int32 Index = 0;
		bool bCanceled = false;
	};

	template <typename TArrayType, typename ElementType>
	struct TDelayState : public FDelayStateBase
	{
		const TArrayType* ArrayPtr = nullptr;
		TFunction<void(ElementType)> Callback;

		void ProcessNext(TSharedRef<TDelayState> Self, FTimerHandle& TimerHandle)
		{
			if (bCanceled || !WorldContextObject.IsValid() || !ArrayPtr || Index >= ArrayPtr->Num())
				return;

			Callback((*ArrayPtr)[Index]);
			Index++;

			if (Index >= ArrayPtr->Num() || bCanceled) return;

			if (UWorld* World = WorldContextObject->GetWorld())
			{
				World->GetTimerManager().SetTimer(
					TimerHandle,
					FTimerDelegate::CreateLambda([Self, &TimerHandle]()
					{
						Self->ProcessNext(Self, TimerHandle);
					}),
					Delay,
					false
				);
			}
		}
	};

	// 泛型控制器，持有状态和定时器
	struct DREAMANIMATIONTEXTBLOCK_API FForEachWithDelayHandle
	{
		FTimerHandle TimerHandle;
		TSharedPtr<FDelayStateBase> State;

		void Stop()
		{
			if (State.IsValid())
			{
				State->bCanceled = true;
				if (UWorld* World = State->WorldContextObject->GetWorld())
				{
					World->GetTimerManager().ClearTimer(TimerHandle);
				}
			}
		}
	};

	template <typename TArrayType, typename ElementType>
	static FForEachWithDelayHandle ForEachWithDelay(
		UObject* WorldContextObject,
		const TArrayType& Array,
		float DelayBetweenElements,
		TFunction<void(ElementType)> OnElementProcessed
	)
	{
		FForEachWithDelayHandle Handle;

		if (!WorldContextObject || !WorldContextObject->GetWorld() || Array.Num() == 0)
		{
			return Handle;
		}

		TSharedRef<TDelayState<TArrayType, ElementType>> State = MakeShared<TDelayState<TArrayType, ElementType>>();
		State->WorldContextObject = WorldContextObject;
		State->ArrayPtr = &Array;
		State->Callback = OnElementProcessed;
		State->Delay = DelayBetweenElements;
		State->Index = 0;

		Handle.State = StaticCastSharedRef<FDelayStateBase>(State);
		State->ProcessNext(State, Handle.TimerHandle);

		return Handle;
	}


};