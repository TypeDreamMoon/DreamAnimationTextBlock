// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "DreamWidgetTweener.h"
#include "DreamWidgetTweenerVirtual.generated.h"

UCLASS(NotBlueprintType)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenerVirtual:public UDreamWidgetTweener
{
	GENERATED_BODY()
public:

	void SetInitialValue(float newDuration)
	{
		this->duration = newDuration;
	}
protected:
	virtual void OnStartGetValue() override
	{
		
	}
	virtual void TweenAndApplyValue(float currentTime) override
	{
		
	}
	virtual void SetValueForIncremental() override
	{
		
	}
	virtual void SetOriginValueForRestart() override
	{
		
	}
};