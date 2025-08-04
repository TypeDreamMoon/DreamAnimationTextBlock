// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#include "DreamWidgetTweener.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UDreamWidgetTweener::UDreamWidgetTweener()
{
	tweenFunc.BindStatic(&UDreamWidgetTweener::OutCubic);//OutCubic default animation curve function
}
UDreamWidgetTweener* UDreamWidgetTweener::SetEase(EDreamWidgetTweenEase easetype)
{
	if (elapseTime > 0 || startToTween)return this;
	switch (easetype)
	{
	case EDreamWidgetTweenEase::Linear:
		tweenFunc.BindStatic(&UDreamWidgetTweener::Linear);
		break;
	case EDreamWidgetTweenEase::InQuad:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InQuad);
		break;
	case EDreamWidgetTweenEase::OutQuad:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutQuad);
		break;
	case EDreamWidgetTweenEase::InOutQuad:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutQuad);
		break;
	case EDreamWidgetTweenEase::InCubic:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InCubic);
		break;
	case EDreamWidgetTweenEase::OutCubic:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutCubic);
		break;
	case EDreamWidgetTweenEase::InOutCubic:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutCubic);
		break;
	case EDreamWidgetTweenEase::InQuart:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InQuart);
		break;
	case EDreamWidgetTweenEase::OutQuart:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutQuart);
		break;
	case EDreamWidgetTweenEase::InOutQuart:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutQuart);
		break;
	case EDreamWidgetTweenEase::InSine:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InSine);
		break;
	case EDreamWidgetTweenEase::OutSine:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutSine);
		break;
	case EDreamWidgetTweenEase::InOutSine:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutSine);
		break;
	case EDreamWidgetTweenEase::InExpo:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InExpo);
		break;
	case EDreamWidgetTweenEase::OutExpo:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutExpo);
		break;
	case EDreamWidgetTweenEase::InOutExpo:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutExpo);
		break;
	case EDreamWidgetTweenEase::InCirc:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InCirc);
		break;
	case EDreamWidgetTweenEase::OutCirc:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutCirc);
		break;
	case EDreamWidgetTweenEase::InOutCirc:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutCirc);
		break;
	case EDreamWidgetTweenEase::InElastic:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InElastic);
		break;
	case EDreamWidgetTweenEase::OutElastic:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutElastic);
		break;
	case EDreamWidgetTweenEase::InOutElastic:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutElastic);
		break;
	case EDreamWidgetTweenEase::InBack:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InBack);
		break;
	case EDreamWidgetTweenEase::OutBack:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutBack);
		break;
	case EDreamWidgetTweenEase::InOutBack:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutBack);
		break;
	case EDreamWidgetTweenEase::InBounce:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InBounce);
		break;
	case EDreamWidgetTweenEase::OutBounce:
		tweenFunc.BindStatic(&UDreamWidgetTweener::OutBounce);
		break;
	case EDreamWidgetTweenEase::InOutBounce:
		tweenFunc.BindStatic(&UDreamWidgetTweener::InOutBounce);
		break;
	case EDreamWidgetTweenEase::CurveFloat:
		tweenFunc.BindUObject(this, &UDreamWidgetTweener::CurveFloat);
		break;
	}
	return this;
}
UDreamWidgetTweener* UDreamWidgetTweener::SetDelay(float newDelay)
{
	if (elapseTime > 0 || startToTween)return this;
	this->delay = newDelay;
	if (this->delay < 0)
	{
		this->delay = 0;
	}
	return this;
}
UDreamWidgetTweener* UDreamWidgetTweener::SetLoop(EDreamWidgetTweenLoop newLoopType, int32 newLoopCount)
{
	if (elapseTime > 0 || startToTween)return this;
	this->loopType = newLoopType;
	this->maxLoopCount = newLoopCount;
	return this;
}
UDreamWidgetTweener* UDreamWidgetTweener::SetEaseCurve(UCurveFloat* newCurve)
{
	if (IsValid(newCurve))
	{
		SetEase(EDreamWidgetTweenEase::CurveFloat);
		curveFloat = newCurve;
	}
	else
	{
		
	}
	return this;
}

UDreamWidgetTweener* UDreamWidgetTweener::SetCurveFloat(UCurveFloat* newCurveFloat)
{
	if (elapseTime > 0 || startToTween)return this;
	curveFloat = newCurveFloat;
	return this;
}
UDreamWidgetTweener* UDreamWidgetTweener::SetAffectByGamePause(bool value)
{
	affectByGamePause = value;
	return this;
}
UDreamWidgetTweener* UDreamWidgetTweener::SetAffectByTimeDilation(bool value)
{
	affectByTimeDilation = value;
	return this;
}

bool UDreamWidgetTweener::ToNext(float deltaTime, float unscaledDeltaTime)
{
	if (auto world = GetWorld())
	{
		if (world->IsPaused() && affectByGamePause)return true;
	}
	if (isMarkedToKill)return false;
	if (isMarkedPause)return true;//no need to tick time if pause
	return this->ToNextWithElapsedTime(elapseTime + (affectByTimeDilation ? deltaTime : unscaledDeltaTime));
}
bool UDreamWidgetTweener::ToNextWithElapsedTime(float InElapseTime)
{
	this->elapseTime = InElapseTime;
	if (elapseTime > delay)//if elapseTime bigger than delay, do animation
	{
		if (!startToTween)
		{
			startToTween = true;
			//set initialize value
			OnStartGetValue();
			//execute callback
			onCycleStartCpp.ExecuteIfBound();
			onStartCpp.ExecuteIfBound();
		}

		float elapseTimeWithoutDelay = elapseTime - delay;
		float currentTime = elapseTimeWithoutDelay - duration * loopCycleCount;
		if (currentTime >= duration)
		{
			bool returnValue = true;
			loopCycleCount++;

			TweenAndApplyValue(reverseTween ? 0 : duration);
			onUpdateCpp.ExecuteIfBound(1.0f);
			onCycleCompleteCpp.ExecuteIfBound();
			if (loopType == EDreamWidgetTweenLoop::Once)
			{
				onCompleteCpp.ExecuteIfBound();
				returnValue = false;
			}
			else if (maxLoopCount <= -1)//infinite loop
			{
				onCycleStartCpp.ExecuteIfBound();//start new cycle callback
				returnValue = true;
			}
			else
			{
				if (loopCycleCount >= maxLoopCount)//reach end cycle
				{
					onCompleteCpp.ExecuteIfBound();
					returnValue = false;
				}
				else//not reach end cycle
				{
					onCycleStartCpp.ExecuteIfBound();//start new cycle callback
					returnValue = true;
				}
			}
			switch (loopType)
			{
			case EDreamWidgetTweenLoop::Restart:
			{
				SetValueForRestart();
			}
			break;
			case EDreamWidgetTweenLoop::Yoyo:
			{
				reverseTween = !reverseTween;
				SetValueForYoyo();
			}
			break;
			case EDreamWidgetTweenLoop::Incremental:
			{
				SetValueForIncremental();
			}
			break;
			}
			return returnValue;
		}
		else
		{
			if (reverseTween)
			{
				currentTime = duration - currentTime;
			}
			TweenAndApplyValue(currentTime);
			onUpdateCpp.ExecuteIfBound(currentTime / duration);
			return true;
		}
	}
	else
	{
		return true;//waiting delay
	}
}

void UDreamWidgetTweener::Kill(bool callComplete)
{
	if (callComplete)
	{
		onCompleteCpp.ExecuteIfBound();
	}
	isMarkedToKill = true;
}

void UDreamWidgetTweener::ForceComplete()
{
	isMarkedToKill = true;
	elapseTime = delay + duration;
	TweenAndApplyValue(duration);
	onUpdateCpp.ExecuteIfBound(1.0f);
	onCompleteCpp.ExecuteIfBound();
}

void UDreamWidgetTweener::Restart()
{
	if (elapseTime == 0)
	{
		return;
	}
	isMarkedPause = false;//incase it is paused.
	//reset parameter to initial
	loopCycleCount = 0;
	reverseTween = false;
	SetOriginValueForRestart();

	this->ToNextWithElapsedTime(0);
}

void UDreamWidgetTweener::Goto(float timePoint)
{
	timePoint = FMath::Clamp(timePoint, 0.0f, duration);
	//reset parameter to initial
	loopCycleCount = 0;
	reverseTween = false;

	this->ToNextWithElapsedTime(timePoint);
}

float UDreamWidgetTweener::GetProgress()const
{
	if (elapseTime > delay)
	{
		float elapseTimeWithoutDelay = elapseTime - delay;
		float currentTime = elapseTimeWithoutDelay - duration * loopCycleCount;
		if (currentTime >= duration)
		{
			return 1;
		}
		else
		{
			if (reverseTween)
			{
				currentTime = duration - currentTime;
			}
			return currentTime / duration;
		}
	}
	else
	{
		return 0;
	}
}

UDreamWidgetTweener* UDreamWidgetTweener::SetTickType(EDreamWidgetTweenTickType value)
{
	if (elapseTime > 0 || startToTween)return this;
	this->tickType = value;
	return this;
}

float UDreamWidgetTweener::CurveFloat(float c, float b, float t, float d)
{
	if (d < KINDA_SMALL_NUMBER)return c + b;
	if (curveFloat.IsValid())
	{
		return curveFloat->GetFloatValue(t / d) * c + b;
	}
	else
	{
		return Linear(c, b, t, d);
	}
}