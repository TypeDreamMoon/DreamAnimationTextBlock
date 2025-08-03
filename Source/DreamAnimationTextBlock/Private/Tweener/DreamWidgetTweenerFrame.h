// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "DreamWidgetTweener.h"
#include "Engine/World.h"
#include "DreamWidgetTweenerFrame.generated.h"

UCLASS(NotBlueprintType)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenerFrame : public UDreamWidgetTweener
{
	GENERATED_BODY()

public:
	uint32 startFrameNumber = 0; //b
	uint32 endFrameNumber = 0; //c

	void SetInitialValue(int newEndValue)
	{
		this->startFrameNumber = GFrameNumber;
		this->endFrameNumber = GFrameNumber + newEndValue;
	}

protected:
	virtual void OnStartGetValue() override
	{
	}

	virtual bool ToNext(float deltaTime, float unscaledDeltaTime) override
	{
		if (auto world = GetWorld())
		{
			if (world->IsPaused() && affectByGamePause)return true;
		}
		if (isMarkedToKill)return false;
		if (isMarkedPause)return true; //no need to tick time if pause
		if (!startToTween)
		{
			startToTween = true;
			onStartCpp.ExecuteIfBound();
		}

		if (GFrameNumber >= endFrameNumber)
		{
			onUpdateCpp.ExecuteIfBound(1.0f);
			onCompleteCpp.ExecuteIfBound();
			return false;
		}
		else
		{
			onUpdateCpp.ExecuteIfBound((float)(GFrameNumber - startFrameNumber) / (endFrameNumber - startFrameNumber));
			return true;
		}
	}

	virtual void TweenAndApplyValue(float currentTime) override
	{
	}

	virtual UDreamWidgetTweener* SetDelay(float newDelay) override
	{
		return this;
	}

	virtual UDreamWidgetTweener* SetLoop(EDreamWidgetTweenLoop newLoopType, int32 newLoopCount) override
	{
		return this;
	}

	virtual void SetValueForIncremental() override
	{
	}

	virtual void SetOriginValueForRestart() override
	{
		auto changeValue = endFrameNumber - startFrameNumber;
		startFrameNumber = GFrameNumber;
		endFrameNumber = GFrameNumber + changeValue;
	}
};
