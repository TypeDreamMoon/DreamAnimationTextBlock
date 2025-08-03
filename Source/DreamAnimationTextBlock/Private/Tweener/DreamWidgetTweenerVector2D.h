// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "DreamWidgetTweener.h"
#include "DreamWidgetTweenerVector2D.generated.h"

UCLASS(NotBlueprintType)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenerVector2D :public UDreamWidgetTweener
{
	GENERATED_BODY()
public:
	float startFloat = 0.0f;//b
	float changeFloat = 1.0f;//c
	FVector2D startValue;
	FVector2D endValue;

	FDreamWidgetTweenVector2DGetterFunction getter;
	FDreamWidgetTweenVector2DSetterFunction setter;

	FVector2D originStartValue;

	void SetInitialValue(const FDreamWidgetTweenVector2DGetterFunction& newGetter, const FDreamWidgetTweenVector2DSetterFunction& newSetter, const FVector2D& newEndValue, float newDuration)
	{
		this->duration = newDuration;
		this->getter = newGetter;
		this->setter = newSetter;
		this->endValue = newEndValue;

		this->startFloat = 0.0f;
		this->changeFloat = 1.0f;
	}
protected:
	virtual void OnStartGetValue() override
	{
		if (getter.IsBound())
			this->startValue = getter.Execute();
		this->originStartValue = this->startValue;
	}
	virtual void TweenAndApplyValue(float currentTime) override
	{
		float lerpValue = tweenFunc.Execute(changeFloat, startFloat, currentTime, duration);
		auto value = FMath::Lerp(startValue, endValue, lerpValue);
		setter.ExecuteIfBound(value);
	}
	virtual void SetValueForIncremental() override
	{
		auto diffValue = endValue - startValue;
		startValue = endValue;
		endValue += diffValue;
	}
	virtual void SetOriginValueForRestart() override
	{
		auto diffValue = endValue - startValue;
		startValue = originStartValue;
		endValue = originStartValue + diffValue;
	}
};