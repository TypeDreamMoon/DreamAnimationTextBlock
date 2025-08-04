// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
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
		TFunction<void()> CompletedCallback;

		void ProcessNext(TSharedRef<TDelayState> Self, FTimerHandle& TimerHandle)
		{
			// 如果取消了，则直接执行完成回调
			if (bCanceled || !WorldContextObject.IsValid() || !ArrayPtr || Index >= ArrayPtr->Num())
			{
				CompletedCallback();
				return;
			}

			// 执行回调
			Callback((*ArrayPtr)[Index]);
			Index++;

			// 如果已经结束，则直接执行完成回调
			if (Index >= ArrayPtr->Num() || bCanceled)
			{
				CompletedCallback();
				return;
			}

			// 启动下一次的定时器
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
		TFunction<void(ElementType)> OnElementProcessed,
		TFunction<void()> OnCompletedProcessed = nullptr
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
		State->CompletedCallback = OnCompletedProcessed;
		State->Delay = DelayBetweenElements;
		State->Index = 0;

		Handle.State = StaticCastSharedRef<FDelayStateBase>(State);
		State->ProcessNext(State, Handle.TimerHandle);

		return Handle;
	}


};