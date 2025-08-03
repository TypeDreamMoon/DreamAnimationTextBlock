// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#include "DreamWidgetTweenerSequence.h"
#include "DreamWidgetTweener.h"
#include "DreamWidgetTweenManager.h"
#include "Tweener/DreamWidgetTweenerFrame.h"
#include "Tweener/DreamWidgetTweenerVirtual.h"

UDreamWidgetTweenerSequence* UDreamWidgetTweenerSequence::Append(UObject* WorldContextObject, UDreamWidgetTweener* tweener)
{
	return this->Insert(WorldContextObject, duration, tweener);
}
UDreamWidgetTweenerSequence* UDreamWidgetTweenerSequence::AppendInterval(UObject* WorldContextObject, float interval)
{
	if (elapseTime > 0 || startToTween)
	{
		return this;
	}
	duration += interval;
	return this;
}
UDreamWidgetTweenerSequence* UDreamWidgetTweenerSequence::Insert(UObject* WorldContextObject, float timePosition, UDreamWidgetTweener* tweener)
{
	if (!IsValid(tweener))
	{
		return this;
	}
	if (tweener->IsA<UDreamWidgetTweenerFrame>() || tweener->IsA<UDreamWidgetTweenerVirtual>())
	{
		return this;
	}
	if (elapseTime > 0 || startToTween)
	{
		return this;
	}
	if (tweenerList.Contains(tweener))
	{
		return this;
	}
	if (tweener->loopType != EDreamWidgetTweenLoop::Once && tweener->maxLoopCount == -1)
	{
		tweener->maxLoopCount = 1;
	}
	UDreamWidgetTweenManager::RemoveTweener(WorldContextObject, tweener);
	int loopCount = tweener->loopType == EDreamWidgetTweenLoop::Once ? 1 : tweener->maxLoopCount;
	float tweenerTime = tweener->delay + tweener->duration * loopCount;
	tweener->SetDelay(tweener->delay + timePosition);
	tweenerList.Add(tweener);
	lastTweenStartTime = timePosition;
	float inputDuration = tweenerTime + timePosition;
	if (duration < inputDuration)
	{
		duration = inputDuration;
	}
	return this;
}
UDreamWidgetTweenerSequence* UDreamWidgetTweenerSequence::Prepend(UObject* WorldContextObject, UDreamWidgetTweener* tweener)
{
	if (!IsValid(tweener))
	{
		return this;
	}
	if (tweener->IsA<UDreamWidgetTweenerFrame>() || tweener->IsA<UDreamWidgetTweenerVirtual>())
	{
		return this;
	}
	if (elapseTime > 0 || startToTween)
	{
		return this;
	}
	if (tweenerList.Contains(tweener))
	{
		return this;
	}
	if (tweener->loopType != EDreamWidgetTweenLoop::Once && tweener->maxLoopCount == -1)
	{
		tweener->maxLoopCount = 1;
	}
	UDreamWidgetTweenManager::RemoveTweener(WorldContextObject, tweener);
	int loopCount = tweener->loopType == EDreamWidgetTweenLoop::Once ? 1 : tweener->maxLoopCount;
	float inputDuration = tweener->delay + tweener->duration * loopCount;
	//offset others 
	for (auto& item : tweenerList)
	{
		item->SetDelay(item->delay + inputDuration);
	}
	tweenerList.Insert(tweener, 0);
	duration += inputDuration;
	lastTweenStartTime = 0;
	return this;
}
UDreamWidgetTweenerSequence* UDreamWidgetTweenerSequence::PrependInterval(UObject* WorldContextObject, float interval)
{
	if (elapseTime > 0 || startToTween)
	{
		return this;
	}
	//offset others 
	for (auto& item : tweenerList)
	{
		item->SetDelay(item->delay + interval);
	}
	duration += interval;
	lastTweenStartTime += interval;
	return this;
}
UDreamWidgetTweenerSequence* UDreamWidgetTweenerSequence::Join(UObject* WorldContextObject, UDreamWidgetTweener* tweener)
{
	if (!IsValid(tweener))
	{
		return this;
	}
	if (tweener->IsA<UDreamWidgetTweenerFrame>() || tweener->IsA<UDreamWidgetTweenerVirtual>())
	{
		return this;
	}
	if (tweenerList.Num() == 0)return this;
	return this->Insert(WorldContextObject, lastTweenStartTime, tweener);
}

void UDreamWidgetTweenerSequence::TweenAndApplyValue(float currentTime)
{
	for(int i = 0; i < tweenerList.Num(); i++)
	{
		auto& item = tweenerList[i];
		if (!item->ToNextWithElapsedTime(currentTime))
		{
			finishedTweenerList.Add(item);
			tweenerList.RemoveAt(i);
			i--;
		}
	}
}

void UDreamWidgetTweenerSequence::SetOriginValueForRestart()
{
	for (auto& item : finishedTweenerList)
	{
		//add tweener to tweenerList
		tweenerList.Add(item);
	}
	finishedTweenerList.Reset();

	for (auto& item : tweenerList)
	{
		if (item->elapseTime > 0 || item->startToTween)
		{
			item->SetOriginValueForRestart();//if tween already start, then we can call "SetOriginValueForRestart"
			item->TweenAndApplyValue(0);
		}
		//set parameter to initial
		item->elapseTime = 0;
		item->loopCycleCount = 0;
		item->reverseTween = false;
	}
}

void UDreamWidgetTweenerSequence::SetValueForIncremental()
{
	for (auto& item : finishedTweenerList)
	{
		item->SetValueForIncremental();
		//set parameter to initial
		item->elapseTime = 0;
		item->loopCycleCount = 0;
		item->reverseTween = false;
		item->TweenAndApplyValue(0);

		//add tweener to tweenerList
		tweenerList.Add(item);
	}
	finishedTweenerList.Reset();
}
void UDreamWidgetTweenerSequence::SetValueForYoyo()
{
	this->reverseTween = !this->reverseTween;//reverse it again, so it will keep value false, because we only need to reverse tweenerList
	for (auto& item : finishedTweenerList)
	{
		if (item->loopType != EDreamWidgetTweenLoop::Yoyo)//if it is already yoyo, then we no need to change reverseTween for it
		{
			item->reverseTween = !item->reverseTween;
		}

		//set parameter to initial
		item->elapseTime = 0;
		item->loopCycleCount = 0;
		//flip tweener
		int loopCount = item->loopType == EDreamWidgetTweenLoop::Once ? 1 : item->maxLoopCount;
		float tweenerDelay = duration - (item->delay + item->duration * loopCount);
		item->delay = tweenerDelay;

		//add tweener to tweenerList
		tweenerList.Add(item);
	}
	finishedTweenerList.Reset();
}
void UDreamWidgetTweenerSequence::SetValueForRestart()
{
	for (auto& item : finishedTweenerList)
	{
		//set parameter to initial
		item->elapseTime = 0;
		item->loopCycleCount = 0;
		item->reverseTween = false;
		item->TweenAndApplyValue(0);

		//add tweener to tweenerList
		tweenerList.Add(item);
	}
	finishedTweenerList.Reset();
}
void UDreamWidgetTweenerSequence::Restart()
{
	if (elapseTime == 0)
	{
		return;
	}
	this->isMarkedPause = false;//incase it is paused.

	//reset parameter and value to start
	{
		//reset parameter to initial
		if (this->loopType == EDreamWidgetTweenLoop::Yoyo)
		{
			if (loopCycleCount % 2 != 0)//this means current is yoyo back, then we should reverse it
			{
				this->reverseTween = true;
				for (auto& item : tweenerList)
				{
					finishedTweenerList.Add(item);
				}
				tweenerList.Reset();
				this->SetValueForYoyo();
			}
		}
		for (auto& item : finishedTweenerList)
		{
			tweenerList.Add(item);
		}
		finishedTweenerList.Reset();
		this->loopCycleCount = 0;

		//sort it, so later tweener can do "SetOriginValueForRestart" ealier, so ealier tweener will get correct start state
		tweenerList.Sort([=](const UDreamWidgetTweener& A, const UDreamWidgetTweener& B) {
			return A.delay > B.delay;
			});
		for (int i = 0; i < tweenerList.Num(); i++)
		{
			auto& item = tweenerList[i];
			if (item->startToTween)
			{
				item->SetOriginValueForRestart();
				item->TweenAndApplyValue(0);
			}
			//set parameter to initial
			item->elapseTime = 0;
			item->loopCycleCount = 0;
			item->reverseTween = false;
		}
	}

	this->ToNextWithElapsedTime(0);
}
void UDreamWidgetTweenerSequence::Goto(float timePoint)
{
	timePoint = FMath::Clamp(timePoint, 0.0f, duration);

	//reset parameter to start, then goto timepoint. these line should be same as lines in "Restart"
	{
		//reset parameter to initial
		if (this->loopType == EDreamWidgetTweenLoop::Yoyo)
		{
			if (loopCycleCount % 2 != 0)//mean current is yoyo back, should reverse it
			{
				this->reverseTween = true;
				for (auto& item : tweenerList)
				{
					finishedTweenerList.Add(item);
				}
				tweenerList.Reset();
				this->SetValueForYoyo();
			}
		}
		for (auto& item : finishedTweenerList)
		{
			tweenerList.Add(item);
		}
		finishedTweenerList.Reset();
		this->loopCycleCount = 0;

		//sort it, so later tweener can do "SetOriginValueForRestart" ealier, so ealier tweener will get correct start state
		tweenerList.Sort([=](const UDreamWidgetTweener& A, const UDreamWidgetTweener& B) {
			return A.delay > B.delay;
			});
		for (int i = 0; i < tweenerList.Num(); i++)
		{
			auto& item = tweenerList[i];
			if (item->startToTween)
			{
				item->SetOriginValueForRestart();
				item->TweenAndApplyValue(0);
			}
			//set parameter to initial
			item->elapseTime = 0;
			item->loopCycleCount = 0;
			item->reverseTween = false;
		}
	}

	this->ToNextWithElapsedTime(timePoint);
}

