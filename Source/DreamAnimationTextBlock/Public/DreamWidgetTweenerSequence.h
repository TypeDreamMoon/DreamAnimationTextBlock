// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "DreamWidgetTweener.h"
#include "DreamWidgetTweenerSequence.generated.h"

UCLASS(BlueprintType)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenerSequence:public UDreamWidgetTweener
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = LTween)TArray<TObjectPtr<UDreamWidgetTweener>> tweenerList;
	UPROPERTY(VisibleAnywhere, Category = LTween)TArray<TObjectPtr<UDreamWidgetTweener>> finishedTweenerList;
	float lastTweenStartTime = 0;
public:
	/**
	 * Adds the given tween to the end of the Sequence.
	 * Not support Tweener type: Delay/ DelayFrame/ Virtual.
	 * Has no effect if the Sequence has already started.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = LTween)
		UDreamWidgetTweenerSequence* Append(UObject* WorldContextObject, UDreamWidgetTweener* tweener);
	/**
	 * Adds the given interval to the end of the Sequence.
	 * Has no effect if the Sequence has already started.
	 * @param interval The interval duration
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = LTween)
		UDreamWidgetTweenerSequence* AppendInterval(UObject* WorldContextObject, float interval);
	/**
	 * Inserts the given tween at the given time position in the Sequence, automatically adding an interval if needed.
	 * Not support Tweener type: Delay/ DelayFrame/ Virtual.
	 * Has no effect if the Sequence has already started.
	 * @param timePosition The time position where the tween will be placed
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = LTween)
		UDreamWidgetTweenerSequence* Insert(UObject* WorldContextObject, float timePosition, UDreamWidgetTweener* tweener);
	/**
	 * Adds the given tween to the beginning of the Sequence, pushing forward the other nested content.
	 * Not support Tweener type: Delay/ DelayFrame/ Virtual.
	 * Has no effect if the Sequence has already started.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = LTween)
		UDreamWidgetTweenerSequence* Prepend(UObject* WorldContextObject, UDreamWidgetTweener* tweener);
	/**
	 * Adds the given interval to the beginning of the Sequence, pushing forward the other nested content.
	 * Has no effect if the Sequence has already started.
	 * @param interval The interval duration
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = LTween)
		UDreamWidgetTweenerSequence* PrependInterval(UObject* WorldContextObject, float interval);
	/**
	 * Inserts the given tween at the same time position of the last tween added to the Sequence.
	 * Note that, in case of a Join after an interval, the insertion time will be the time where the interval starts, not where it finishes.
	 * Not support Tweener type: Delay/ DelayFrame/ Virtual.
	 * Has no effect if the Sequence has already started.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = LTween)
		UDreamWidgetTweenerSequence* Join(UObject* WorldContextObject, UDreamWidgetTweener* tweener);

protected:
	virtual void OnStartGetValue() override {};
	virtual void TweenAndApplyValue(float currentTime) override;
	virtual void SetValueForIncremental() override;
	virtual void SetValueForYoyo() override;
	virtual void SetValueForRestart() override;
	virtual void SetOriginValueForRestart() override;

	virtual void Restart()override;
	virtual void Goto(float timePoint)override;
};