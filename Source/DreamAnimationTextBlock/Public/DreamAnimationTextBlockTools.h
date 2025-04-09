#pragma once

#include "CoreMinimal.h"

struct DREAMANIMATIONTEXTBLOCK_API FDreamAnimationTextBlockTools
{
public:
	// TODO : 这里遍历有些问题 只能访问前两个元素 待解决 BUG
	template <typename TArrayType, typename ElementType>
	static void ForEachWithDelay(
		UObject* WorldContextObject,
		const TArrayType& Array,
		float DelayBetweenElements,
		TFunction<void(ElementType)> OnElementProcessed
	)
	{
		if (!WorldContextObject || !WorldContextObject->GetWorld()) return;

		// 使用共享指针管理状态
		struct FDelayState
		{
			int32 CurrentIndex = 0;
			FTimerHandle TimerHandle;
			TFunction<void()> ProcessNext; // 将ProcessNext移至状态结构
		};

		TSharedPtr<FDelayState> State = MakeShared<FDelayState>();

		// 定义处理逻辑
		State->ProcessNext = [WorldContextObject, &Array, DelayBetweenElements, OnElementProcessed, State]()
		{
			if (State->CurrentIndex >= Array.Num()) return;

			// 处理当前元素
			OnElementProcessed(Array[State->CurrentIndex]);
			State->CurrentIndex++;

			// 设置下一个元素的定时器
			if (State->CurrentIndex < Array.Num())
			{
				WorldContextObject->GetWorld()->GetTimerManager().SetTimer(
					State->TimerHandle,
					MoveTemp(State->ProcessNext), // 关键修改：转换为右值引用
					DelayBetweenElements,
					false
				);
			}
		};

		// 启动第一个元素
		State->ProcessNext();
	}
};
