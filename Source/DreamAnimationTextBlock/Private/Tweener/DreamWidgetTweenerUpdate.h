// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "DreamWidgetTweener.h"
#include "Engine/World.h"
#include "DreamWidgetTweenerUpdate.generated.h"

UCLASS(NotBlueprintType)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenerUpdate :public UDreamWidgetTweener
{
	GENERATED_BODY()
public:
	void SetInitialValue(int newEndValue)
	{
		
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
		if (isMarkedPause)return true;//no need to tick time if pause
		onUpdateCpp.ExecuteIfBound(affectByTimeDilation ? deltaTime : unscaledDeltaTime);
		return true;
	}
	virtual void TweenAndApplyValue(float currentTime) override
	{
		
	}
	virtual UDreamWidgetTweener* SetDelay(float newDelay)override
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
		
	}
};