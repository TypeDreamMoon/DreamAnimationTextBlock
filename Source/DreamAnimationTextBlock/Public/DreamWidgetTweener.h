// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/HitResult.h"
#include "DreamWidgetTweener.generated.h"

DECLARE_DELEGATE_RetVal_FourParams(float, FDreamWidgetTweenFunction, float, float, float, float);

DECLARE_DELEGATE_OneParam(FDreamWidgetTweenUpdateDelegate, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FDreamWidgetTweenUpdateMulticastDelegate, float);

#ifndef DreamWidgetTweenUpdateDelegate
#define DreamWidgetTweenUpdateDelegate UE_DEPRECATED_MACRO(5.0, "DreamWidgetTweenUpdateDelegate has been renamed to FDreamWidgetTweenUpdateDelegate") FDreamWidgetTweenUpdateDelegate
#endif
#ifndef DreamWidgetTweenUpdateMulticastDelegate
#define DreamWidgetTweenUpdateMulticastDelegate UE_DEPRECATED_MACRO(5.0, "DreamWidgetTweenUpdateMulticastDelegate has been renamed to FDreamWidgetTweenUpdateMulticastDelegate") FDreamWidgetTweenUpdateMulticastDelegate
#endif

DECLARE_DELEGATE_RetVal(float, FDreamWidgetTweenFloatGetterFunction);
DECLARE_DELEGATE_OneParam(FDreamWidgetTweenFloatSetterFunction, float);

DECLARE_DELEGATE_RetVal(double, FDreamWidgetTweenDoubleGetterFunction);
DECLARE_DELEGATE_OneParam(FDreamWidgetTweenDoubleSetterFunction, double);

DECLARE_DELEGATE_RetVal(FVector, FDreamWidgetTweenVectorGetterFunction);
DECLARE_DELEGATE_OneParam(FDreamWidgetTweenVectorSetterFunction, FVector);

DECLARE_DELEGATE_RetVal(FVector2D, FDreamWidgetTweenVector2DGetterFunction);
DECLARE_DELEGATE_OneParam(FDreamWidgetTweenVector2DSetterFunction, FVector2D);

/** simple delegate */
DECLARE_DYNAMIC_DELEGATE(FDreamWidgetTweenerSimpleDynamicDelegate);
/** @param InProgress Progress of this tween, from 0 to 1 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FDreamWidgetTweenerFloatDynamicDelegate, float, InProgress);

/**  */
UENUM(BlueprintType, Category = DreamWidgetTween)
enum class EDreamWidgetTweenTickType:uint8
{
	PrePhysics = ETickingGroup::TG_PrePhysics UMETA(DisplayName = "Pre Physics"),
	DuringPhysics = ETickingGroup::TG_DuringPhysics UMETA(DisplayName = "During Physics"),
	PostPhysics = ETickingGroup::TG_PostPhysics UMETA(DisplayName = "Post Physics"),
	PostUpdateWork = ETickingGroup::TG_PostUpdateWork UMETA(DisplayName = "Post Update Work"),
	/** This tween will use manual tick. You need to call DreamWidgetTweenManager::ManualTick to make it work. */
	Manual,
};

/**
 * Animation curve type
 */
UENUM(BlueprintType, Category = DreamWidgetTween)
enum class EDreamWidgetTweenEase :uint8
{
	Linear,
	InQuad,
	OutQuad,
	InOutQuad,
	InCubic,
	OutCubic,
	InOutCubic,
	InQuart,
	OutQuart,
	InOutQuart,
	InSine,
	OutSine,
	InOutSine,
	InExpo,
	OutExpo,
	InOutExpo,
	InCirc,
	OutCirc,
	InOutCirc,
	InElastic,
	OutElastic,
	InOutElastic,
	InBack,
	OutBack,
	InOutBack,
	InBounce,
	OutBounce,
	InOutBounce,
	/** Use CurveFloat to animate, only range 0-1 is valid. If use this you must assign curveFloat, or fallback to Linear. */
	CurveFloat,
};

#ifndef DreamWidgetTweenEase
#define DreamWidgetTweenEase UE_DEPRECATED_MACRO(5.0, "DreamWidgetTweenEase has been renamed to EDreamWidgetTweenEase") EDreamWidgetTweenEase
#endif

/**
 * Loop type
 */
UENUM(BlueprintType, Category = LGUI)
enum class EDreamWidgetTweenLoop :uint8
{
	/** Play once, not loop */
	Once, 
	/** Each loop cycle restarts from beginning */
	Restart, 
	/** The tween move forward and backward at alternate cycles */
	Yoyo, 
	/** Continuously increments the tween at the end of each loop cycle (A to B, B to B+(A-B), and so on). */
	Incremental,
};

#ifndef DreamWidgetTweenLoop
#define DreamWidgetTweenLoop UE_DEPRECATED_MACRO(5.0, "DreamWidgetTweenLoop has been renamed to EDreamWidgetTweenLoop") EDreamWidgetTweenLoop
#endif

class UCurveFloat;

/** Class for manage single tween */
UCLASS(BlueprintType, Abstract)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweener : public UObject
{
	GENERATED_BODY()

public:
	UDreamWidgetTweener();

protected:
	friend class UDreamWidgetTweenerSequence;
	/** animation duration */
	float duration = 0.0f;
	/** delay time before animation start */
	float delay = 0.0f;
	/** total elapse time, include delay */
	float elapseTime = 0.0f;
	/** use CurveFloat as animation function,horizontal is time (0-1),vertical is value (0-1) */
	TWeakObjectPtr<UCurveFloat> curveFloat = nullptr;
	/** loop type */
	EDreamWidgetTweenLoop loopType = EDreamWidgetTweenLoop::Once;
	/** max loop count when loop type is Restart/Yoyo/Incremental */
	int32 maxLoopCount = 0;
	/** current completed cycle count */
	int32 loopCycleCount = 0;
	/** how this tween update */
	EDreamWidgetTweenTickType tickType = EDreamWidgetTweenTickType::DuringPhysics;

	/** reverse animation */
	bool reverseTween = false;
	/** if animation start play */
	bool startToTween = false;
	/** mark this tween for kill, so when the next update came, the tween will be killed */
	bool isMarkedToKill = false;
	/** mark this tween as pause, it will keep pause until call Resume() */
	bool isMarkedPause = false;
	/** will this tween be affected when GamePause? usually set to false for UI */
	bool affectByGamePause = true;
	/** will this tween use dilation-time or real-time? */
	bool affectByTimeDilation = true;

	/** tween function */
	FDreamWidgetTweenFunction tweenFunc;

	/** call once after animation complete */
	FSimpleDelegate onCompleteCpp;
	/** if use loop, this will call every time when begin tween in every cycle */
	FSimpleDelegate onCycleStartCpp;
	/** if use loop, this will call every time after tween complete in every cycle */
	FSimpleDelegate onCycleCompleteCpp;
	/** call every frame after animation starts */
	FDreamWidgetTweenUpdateDelegate onUpdateCpp;
	/** call once when animation starts */
	FSimpleDelegate onStartCpp;
public:
	/**
	 * Set animation curve type.
	 * Has no effect if the Tween has already started.
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* SetEase(EDreamWidgetTweenEase easetype);
	/** set ease to CurveFloat and use CurveFloat as animation function, horizontal is time (0-1),vertical is value (0-1) */
	UE_DEPRECATED(4.23, "SetEaseCurve is not valid anymore, use SetEase and SetCurveFloat instead.")
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween", meta = (DeprecatedFunction, DeprecationMessage = "SetEaseCurve is not valid anymore, use SetEase and SetCurveFloat instead."))
		UDreamWidgetTweener* SetEaseCurve(UCurveFloat* newCurve);
	/**
	 * Set delay time before start animation.
	 * Has no effect if the Tween has already started.
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual UDreamWidgetTweener* SetDelay(float newDelay);
	UE_DEPRECATED(4.23, "SetLoopType not valid anymore, use SetLoop instead.")
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween", meta = (DeprecatedFunction, DeprecationMessage = "SetLoopType not valid anymore, use SetLoop instead."))
		virtual UDreamWidgetTweener* SetLoopType(EDreamWidgetTweenLoop newLoopType)
	{
		return SetLoop(newLoopType, -1);
	}
	/**
	 * Set loop of tween.
	 * Has no effect if the Tween has already started.
	 * @param newLoopType	loop type
	 * @param newLoopCount	number of cycles to play (-1 for infinite)
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual UDreamWidgetTweener* SetLoop(EDreamWidgetTweenLoop newLoopType, int32 newLoopCount = 1);
	UE_DEPRECATED(4.23, "GetLoopCount not valid anymore, use GetLoopCycleCount instead.")
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween", meta = (DeprecatedFunction, DeprecationMessage = "GetLoopCount not valid anymore, use GetLoopCycleCount instead."))
		int32 GetLoopCount() { return loopCycleCount; }
	/** curently completed loop cycle count */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		int32 GetLoopCycleCount()const { return loopCycleCount; }

	/** execute when animation complete */
	UDreamWidgetTweener* OnComplete(const FSimpleDelegate& newComplete)
	{
		this->onCompleteCpp = newComplete;
		return this;
	}
	/** execute when animation complete */
	UDreamWidgetTweener* OnComplete(const TFunction<void()>& newComplete)
	{
		if (newComplete != nullptr)
		{
			this->onCompleteCpp.BindLambda(newComplete);
		}
		return this;
	}
	/** execute when animation complete */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* OnComplete(const FDreamWidgetTweenerSimpleDynamicDelegate& newComplete)
	{
		this->onCompleteCpp.BindLambda([newComplete] {
			newComplete.ExecuteIfBound();
		});
		return this;
	}
	
	/** if use loop, this will call every time after tween complete in every cycle */
	UDreamWidgetTweener* OnCycleComplete(const FSimpleDelegate& newCycleComplete)
	{
		this->onCycleCompleteCpp = newCycleComplete;
		return this;
	}
	/** if use loop, this will call every time after tween complete in every cycle */
	UDreamWidgetTweener* OnCycleComplete(const TFunction<void()>& newCycleComplete)
	{
		if (newCycleComplete != nullptr)
		{
			this->onCycleCompleteCpp.BindLambda(newCycleComplete);
		}
		return this;
	}
	/** if use loop, this will call every time after tween complete in every cycle */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* OnCycleComplete(const FDreamWidgetTweenerSimpleDynamicDelegate& newCycleComplete)
	{
		this->onCycleCompleteCpp.BindLambda([newCycleComplete] {
			newCycleComplete.ExecuteIfBound();
			});
		return this;
	}

	/** if use loop, this will call every time when begin tween in every cycle */
	UDreamWidgetTweener* OnCycleStart(const FSimpleDelegate& newCycleStart)
	{
		this->onCycleStartCpp = newCycleStart;
		return this;
	}
	/** if use loop, this will call every time when begin tween in every cycle */
	UDreamWidgetTweener* OnCycleStart(const TFunction<void()>& newCycleStart)
	{
		if (newCycleStart != nullptr)
		{
			this->onCycleStartCpp.BindLambda(newCycleStart);
		}
		return this;
	}
	/** if use loop, this will call every time when begin tween in every cycle */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* OnCycleStart(const FDreamWidgetTweenerSimpleDynamicDelegate& newCycleStart)
	{
		this->onCycleStartCpp.BindLambda([newCycleStart] {
			newCycleStart.ExecuteIfBound();
			});
		return this;
	}

	/** execute every frame if animation is playing */
	UDreamWidgetTweener* OnUpdate(const FDreamWidgetTweenUpdateDelegate& newUpdate)
	{
		this->onUpdateCpp = newUpdate;
		return this;
	}
	/** execute every frame if animation is playing */
	UDreamWidgetTweener* OnUpdate(const TFunction<void(float)>& newUpdate)
	{
		if (newUpdate != nullptr)
		{
			this->onUpdateCpp.BindLambda(newUpdate);
		}
		return this;
	}
	/** execute every frame if animation is playing */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* OnUpdate(const FDreamWidgetTweenerFloatDynamicDelegate& newUpdate)
	{
		this->onUpdateCpp.BindLambda([newUpdate](float progress) {
			newUpdate.ExecuteIfBound(progress);
		});
		return this;
	}
	
	/** execute when animation start*/
	UDreamWidgetTweener* OnStart(const FSimpleDelegate& newStart)
	{
		this->onStartCpp = newStart;
		return this;
	}
	/** execute when animation start, blueprint version*/
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* OnStart(const FDreamWidgetTweenerSimpleDynamicDelegate& newStart)
	{
		this->onStartCpp.BindLambda([newStart] {
			newStart.ExecuteIfBound();
		});
		return this;
	}
	/** execute when animation start, lambda version*/
	UDreamWidgetTweener* OnStart(const TFunction<void()>& newStart)
	{
		if (newStart != nullptr)
		{
			this->onStartCpp.BindLambda(newStart);
		}
		return this;
	}
	/**
	 * Set CurveFloat as animation curve. Make sure ease type is CurveFloat. (Call SetEase function to set ease type)
	 * Has no effect if the Tween has already started.
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* SetCurveFloat(UCurveFloat* newCurveFloat);
	/**
	 * @return false: the tween is complete and need to be killed. true: the tween is still processing.
	 */
	virtual bool ToNext(float deltaTime, float unscaledDeltaTime);
	/**
	 * @return false: the tween is complete and need to be killed. true: the tween is still processing.
	 */
	bool ToNextWithElapsedTime(float InElapseTime);
	/** Force stop this animation. if callComplete = true, will call OnComplete after stop*/
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual void Kill(bool callComplete = false);
	/** Force stop this animation at this frame, set value to end, call OnComplete. */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual void ForceComplete();
	/** Pause this animation. */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		void Pause()
	{
		isMarkedPause = true;
	}
	/** Continue play animation if is paused. */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		void Resume()
	{
		isMarkedPause = false;
	}
	/** Will this tween be affected when GamePause? Default is true, usually set to false for UI. */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		bool GetAffectByGamePause()const { return affectByGamePause; }
	/** Will this tween be affected when GamePause? Default is true, usually set to false for UI. */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* SetAffectByGamePause(bool value);
	/** will this tween use dilated-time or real-time? */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		bool GetAffectByTimeDilation()const { return affectByTimeDilation; }
	/** will this tween use dilated-time or real-time? */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* SetAffectByTimeDilation(bool value);
	/**
	 * Restart animation.
	 * Has no effect if the Tween is not started.
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual void Restart();
	/**
	 * Send the tween to the given position in time.
	 * @param timePoint Time position to reach (if higher than the whole tween duration the tween will simply reach its end).
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual void Goto(float timePoint);
	/** Return progress 0-1 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		virtual float GetProgress()const;
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		float GetElapsedTime()const { return elapseTime; }
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		float GetDuration()const { return duration; }

	/** Return tickType of this tween, default is DuringPhysics. */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		EDreamWidgetTweenTickType GetTickType()const { return tickType; }
	/**
	 * Set TickType of this tween.
	 * Has no effect if the Tween has already started.
	 */
	UFUNCTION(BlueprintCallable, Category = "DreamWidgetTween")
		UDreamWidgetTweener* SetTickType(EDreamWidgetTweenTickType value = EDreamWidgetTweenTickType::DuringPhysics);
protected:
	/** get value when start. child class must override this, check DreamWidgetTweenerFloat for reference */
	virtual void OnStartGetValue() PURE_VIRTUAL(UDreamWidgetTweener::OnStartGetValue, );
	/** set value when tweening. child class must override this, check DreamWidgetTweenerFloat for reference */
	virtual void TweenAndApplyValue(float currentTime) PURE_VIRTUAL(UDreamWidgetTweener::TweenAndApplyValue, );
	/** set start and end value if looptype is Incremental. */
	virtual void SetValueForIncremental() PURE_VIRTUAL(UDreamWidgetTweener::SetValueForIncremental, );
	/** set start and end value before the animation wan't to restart */
	virtual void SetOriginValueForRestart() PURE_VIRTUAL(UDreamWidgetTweener::SetOriginValueForRestart, );
	virtual void SetValueForYoyo() {};
	virtual void SetValueForRestart() {};
#pragma region tweenFunctions
public:
	/**
	* @param c		Change needed in value.
	* @param b		Starting value.
	* @param t		Current time (in frames or seconds).
	* @param d		Expected eaSing duration (in frames or seconds).
	* @return		The correct value.
	*/
	static float Linear(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return c*t / d + b;
	}
	static float InQuad(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d;
		return c*t*t + b;
	}
	static float OutQuad(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d;
		return -c*t*(t - 2) + b;
	}
	static float InOutQuad(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d * 0.5f;
		if (t < 1)
		{
			return c * 0.5f * t*t + b;
		}
		else
		{
			--t;
			return -c * 0.5f * (t*(t - 2) - 1) + b;
		}
	}
	static float InCubic(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d;
		return c*t*t*t + b;
	}
	static float OutCubic(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t = t / d - 1.0f;
		return c*(t*t*t + 1) + b;
	}
	static float InOutCubic(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d * 0.5f;
		if (t < 1)
		{
			return c * 0.5f * t*t*t + b;
		}
		else
		{
			t -= 2;
			return c * 0.5f * (t*t*t + 2) + b;
		}
	}
	static float InQuart(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d;
		return c*t*t*t*t + b;
	}
	static float OutQuart(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t = t / d - 1.0f;
		return -c * (t*t*t*t - 1) + b;
	}
	static float InOutQuart(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d * 0.5f;
		if (t < 1)
		{
			return c * 0.5f * t*t*t*t + b;
		}
		else
		{
			t -= 2;
			return -c * 0.5f * (t*t*t*t - 2) + b;
		}
	}
	static float InSine(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return -c * FMath::Cos(t / d * HALF_PI) + c + b;
	}
	static float OutSine(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return c * FMath::Sin(t / d * HALF_PI) + b;
	}
	static float InOutSine(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return -c * 0.5f * (FMath::Cos(PI * t / d) - 1) + b;
	}
	static float InExpo(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return (t == 0.0f) ? b : c * FMath::Pow(2.0f, 10.0f * (t / d - 1.0f)) + b - c * 0.001f;
	}
	static float OutExpo(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return (t == d) ? b + c : c * 1.001f * (-FMath::Pow(2.0f, -10.0f * t / d) + 1.0f) + b;
	}
	static float InOutExpo(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		if (t == 0) return b;
		if (t == d) return b + c;
		t /= d * 0.5f;
		if (t < 1.0f)
		{
			return c * 0.5f * FMath::Pow(2.0f, 10.0f * (t - 1.0f)) + b - c * 0.0005f;
		}
		else
		{
			return c * 0.5f * 1.0005f * (-FMath::Pow(2.0f, -10.0f * (t - 1.0f)) + 2.0f) + b;
		}
	}
	static float InCirc(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d;
		return -c * (FMath::Sqrt(1.0f - t*t) - 1.0f) + b;
	}
	static float OutCirc(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t = t / d - 1.0f;
		return c * FMath::Sqrt(1.0f - t*t) + b;
	}
	static float InOutCirc(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d * 0.5f;
		if (t < 1)
		{
			return -c * 0.5f * (FMath::Sqrt(1 - t * t) - 1) + b;
		}
		else
		{
			t -= 2.0f;
			return c * 0.5f * (FMath::Sqrt(1.0f - t*t) + 1.0f) + b;
		}
	}
	static float InElastic(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		if (t == 0) return b;
		t /= d;
		if (t == 1) return b + c;
		float p = d*0.3f;
		float s = p / 4;
		float a = c;
		t -= 1.0f;
		return -(a*FMath::Pow(2.0f, 10.0f * t) * FMath::Sin((t*d - s)*(2.0f * PI) / p)) + b;
	}
	static float OutElastic(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		if (t == 0) return b;
		t /= d;
		if (t == 1) return b + c;
		float p = d*0.3f;
		float s = p / 4;
		float a = c;
		return (a*FMath::Pow(2.0f, -10.0f * t) * FMath::Sin((t*d - s)*(2.0f * PI) / p) + c + b);
	}
	static float InOutElastic(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		if (t == 0) return b;
		t /= d * 0.5f;
		if (t == 2) return b + c;
		float p = d*0.3f;
		float s = p / 4;
		float a = c;
		if (t < 1.0f)
		{
			t -= 1.0f;
			return -0.5f*(a*FMath::Pow(2.0f, 10.0f * t) * FMath::Sin((t*d - s)*(2.0f * PI) / p)) + b;
		}
		else
		{
			t -= 1.0f;
			return a * FMath::Pow(2.0f, -10.0f * t) * FMath::Sin((t*d - s)*(2.0f * PI) / p)*0.5f + c + b;
		}
	}
	static float InBack(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		static float s = 1.70158f;
		t /= d;
		return c*t*t*((s + 1)*t - s) + b;
	}
	static float OutBack(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		static float s = 1.70158f;
		t = t / d - 1;
		return c*(t*t*((s + 1)*t + s) + 1) + b;
	}
	static float InOutBack(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		if (t < d * 0.5f) return InBack(c, 0, t * 2, d) * .5f + b;
		else return OutBack(c, 0, t * 2 - d, d) * .5f + c * .5f + b;
	}

	static float OutBounce(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		t /= d;
		if (t < (1.0f / 2.75f)) {
			return c*(7.5625f*t*t) + b;
		}
		else if (t < (2.0f / 2.75f)) {
			t -= (1.5f / 2.75f);
			return c*(7.5625f*t*t + .75f) + b;
		}
		else if (t < (2.5f / 2.75f)) {
			t -= (2.25f / 2.75f);
			return c*(7.5625f*t*t + .9375f) + b;
		}
		else {
			t -= (2.625f / 2.75f);
			return c*(7.5625f*t*t + .984375f) + b;
		}
	}
	static float InBounce(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		return c - OutBounce(c, 0, d - t, d) + b;
	}
	static float InOutBounce(float c, float b, float t, float d)
	{
		if (d < KINDA_SMALL_NUMBER)return c + b;
		if (t < d * 0.5f) return InBounce(c, 0, t * 2, d) * .5f + b;
		else return OutBounce(c, 0, t * 2 - d, d) * .5f + c * .5f + b;
	}
	/** Tween use CurveFloat, in range of 0-1. if curveFloat is null, fallback to Linear */
	float CurveFloat(float c, float b, float t, float d);
#pragma endregion
};