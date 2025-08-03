// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once

#include "CoreMinimal.h"
#include "DreamWidgetTweenManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DreamWidgetTweenDelegateHandleWrapper.h"
#include "DreamWidgetTweenLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDreamWidgetTweenFloatSetterDynamic, float, value);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDreamWidgetTweenDoubleSetterDynamic, double, value);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDreamWidgetTweenVector2SetterDynamic, const FVector2D&, value);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDreamWidgetTweenLinearColorSetterDynamic, const FLinearColor&, value);

UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#pragma region UMG
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "delay,ease", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UMG_RenderTransform_TranslationTo(UObject* WorldContextObject, class UWidget* target, const FVector2D& endValue, float duration = 0.5f, float delay = 0.0f, EDreamWidgetTweenEase ease = EDreamWidgetTweenEase::OutCubic);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "delay,ease", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UMG_RenderTransform_AngleTo(UObject* WorldContextObject, class UWidget* target, float endValue, float duration = 0.5f, float delay = 0.0f, EDreamWidgetTweenEase ease = EDreamWidgetTweenEase::OutCubic);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "delay,ease", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UMG_RenderTransform_ScaleTo(UObject* WorldContextObject, class UWidget* target, const FVector2D& endValue, float duration = 0.5f, float delay = 0.0f, EDreamWidgetTweenEase ease = EDreamWidgetTweenEase::OutCubic);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "delay,ease", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UMG_RenderTransform_ShearTo(UObject* WorldContextObject, class UWidget* target, const FVector2D& endValue, float duration = 0.5f, float delay = 0.0f, EDreamWidgetTweenEase ease = EDreamWidgetTweenEase::OutCubic);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "delay,ease", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UMG_RenderOpacityTo(UObject* WorldContextObject, class UWidget* target, float endValue, float duration = 0.5f, float delay = 0.0f, EDreamWidgetTweenEase ease = EDreamWidgetTweenEase::OutCubic);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "delay,ease", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UMG_DreamAnimTextCharPaddingTo(UObject* WorldContextObject, class UDreamTextChar* target, const FVector2D& endValue, float duration = 0.5f, float delay = 0.0f, EDreamWidgetTweenEase ease = EDreamWidgetTweenEase::OutCubic);
#pragma endregion

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Assign start or update or omplete functions", WorldContext = "WorldContextObject", AutoCreateRefTerm="start,update,complete"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* VirtualCall(UObject* WorldContextObject, float duration, float delay, const FDreamWidgetTweenerSimpleDynamicDelegate& start, const FDreamWidgetTweenerFloatDynamicDelegate& update, const FDreamWidgetTweenerSimpleDynamicDelegate& complete)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, duration);
		if (Tweener)
		{
			Tweener->SetDelay(delay)->OnStart(start)->OnUpdate(update)->OnComplete(complete);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* VirtualCall(UObject* WorldContextObject, float duration, float delay, FSimpleDelegate start, FDreamWidgetTweenUpdateDelegate update, FSimpleDelegate complete)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, duration);
		if (Tweener)
		{
			Tweener->SetDelay(delay)->OnStart(start)->OnUpdate(update)->OnComplete(complete);
		};
		return Tweener;
	}

	static UDreamWidgetTweener* VirtualCall(UObject* WorldContextObject, float duration, float delay, const TFunction<void()>& start, const TFunction<void(float)>& update, const TFunction<void()>& complete)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, duration);
		if (Tweener)
		{
			Tweener->SetDelay(delay)->OnStart(start)->OnUpdate(update)->OnComplete(complete);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* VirtualCall(UObject* WorldContextObject, float duration)
	{
		return UDreamWidgetTweenManager::VirtualTo(WorldContextObject, duration);
	}

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "MainThread delay call function, Assign delayComplete to call", WorldContext = "WorldContextObject", AdvancedDisplay="affectByGamePause,affectByTimeDilation"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* DelayCall(UObject* WorldContextObject, float delayTime, const FDreamWidgetTweenerSimpleDynamicDelegate& delayComplete, bool affectByGamePause = true, bool affectByTimeDilation = true)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, delayTime);
		if (Tweener)
		{
			Tweener->OnComplete(delayComplete)->SetAffectByGamePause(affectByGamePause)->SetAffectByTimeDilation(affectByTimeDilation);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* DelayCall(UObject* WorldContextObject, float delayTime, FSimpleDelegate delayComplete, bool affectByGamePause = true, bool affectByTimeDilation = true)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, delayTime);
		if (Tweener)
		{
			Tweener->OnComplete(delayComplete)->SetAffectByGamePause(affectByGamePause)->SetAffectByTimeDilation(affectByTimeDilation);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* DelayCall(UObject* WorldContextObject, float delayTime, const TFunction<void()>& delayComplete, bool affectByGamePause = true, bool affectByTimeDilation = true)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, delayTime);
		if (Tweener)
		{
			Tweener->OnComplete(delayComplete)->SetAffectByGamePause(affectByGamePause)->SetAffectByTimeDilation(affectByTimeDilation);
		}
		return Tweener;
	}

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "MainThread delay frame call function, Assign delayComplete to call", WorldContext = "WorldContextObject", AdvancedDisplay = "affectByGamePause"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* DelayFrameCall(UObject* WorldContextObject, int frameCount, const FDreamWidgetTweenerSimpleDynamicDelegate& delayComplete, bool affectByGamePause = true)
	{
		auto Tweener = UDreamWidgetTweenManager::DelayFrameCall(WorldContextObject, frameCount);
		if (Tweener)
		{
			Tweener->OnComplete(delayComplete)->SetAffectByGamePause(affectByGamePause);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* DelayFrameCall(UObject* WorldContextObject, int frameCount, FSimpleDelegate delayComplete, bool affectByGamePause = true)
	{
		auto Tweener = UDreamWidgetTweenManager::DelayFrameCall(WorldContextObject, frameCount);
		if (Tweener)
		{
			Tweener->OnComplete(delayComplete)->SetAffectByGamePause(affectByGamePause);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* DelayFrameCall(UObject* WorldContextObject, int frameCount, const TFunction<void()>& delayComplete, bool affectByGamePause = true)
	{
		auto Tweener = UDreamWidgetTweenManager::DelayFrameCall(WorldContextObject, frameCount);
		if (Tweener)
		{
			Tweener->OnComplete(delayComplete)->SetAffectByGamePause(affectByGamePause);
		}
		return Tweener;
	}

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Assign start or update or omplete functions", WorldContext = "WorldContextObject", AutoCreateRefTerm = "start,update,complete"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* UpdateCall(UObject* WorldContextObject, const FDreamWidgetTweenerFloatDynamicDelegate& update)
	{
		auto Tweener = UDreamWidgetTweenManager::UpdateCall(WorldContextObject);
		if (Tweener)
		{
			Tweener->OnUpdate(update);
		}
		return Tweener;
	}

	static UDreamWidgetTweener* UpdateCall(UObject* WorldContextObject, FDreamWidgetTweenUpdateDelegate update)
	{
		auto Tweener = UDreamWidgetTweenManager::UpdateCall(WorldContextObject);
		if (Tweener)
		{
			Tweener->OnUpdate(update);
		};
		return Tweener;
	}

	static UDreamWidgetTweener* UpdateCall(UObject* WorldContextObject, const TFunction<void(float)>& update)
	{
		auto Tweener = UDreamWidgetTweenManager::UpdateCall(WorldContextObject);
		if (Tweener)
		{
			Tweener->OnUpdate(update);
		}
		return Tweener;
	}

	UFUNCTION(BlueprintPure, Category = DreamWidgetTween, meta = (WorldContext = "WorldContextObject"))
	static bool IsTweening(UObject* WorldContextObject, UDreamWidgetTweener* inTweener)
	{
		return UDreamWidgetTweenManager::IsTweening(WorldContextObject, inTweener);
	}

	/**
	 * Force stop this animation. if callComplete = true, OnComplete will call after stop.
	 * This method will check if the tweener is valid.
	 */
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "callComplete", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static void KillIfIsTweening(UObject* WorldContextObject, UDreamWidgetTweener* inTweener, bool callComplete = false)
	{
		UDreamWidgetTweenManager::KillIfIsTweening(WorldContextObject, inTweener, callComplete);
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Kill If Is Tweening (Array)", AdvancedDisplay = "callComplete", WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static void ArrayKillIfIsTweening(UObject* WorldContextObject, const TArray<UDreamWidgetTweener*>& inTweenerArray, bool callComplete = false)
	{
		auto Instance = UDreamWidgetTweenManager::GetDreamWidgetTweenInstance(WorldContextObject);
		if (!IsValid(Instance))return;

		for (auto tweener : inTweenerArray)
		{
			Instance->KillIfIsTweening(WorldContextObject, tweener, callComplete);
		}
	}

	UE_DEPRECATED(5.2, "Use UpdateCall instead.")
	static FDelegateHandle RegisterUpdateEvent(UObject* WorldContextObject, const FDreamWidgetTweenUpdateDelegate& update)
	{
		// Disable deprecation warnings so we can call the deprecated function to support this function (which is also deprecated)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		auto delegateHandle = UDreamWidgetTweenManager::RegisterUpdateEvent(WorldContextObject, update);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		return delegateHandle;
	}

	UE_DEPRECATED(5.2, "Use UpdateCall instead.")
	static FDelegateHandle RegisterUpdateEvent(UObject* WorldContextObject, const TFunction<void(float)>& update)
	{
		FDreamWidgetTweenUpdateDelegate updateDelegate = FDreamWidgetTweenUpdateDelegate::CreateLambda(update);
		// Disable deprecation warnings so we can call the deprecated function to support this function (which is also deprecated)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		UDreamWidgetTweenManager::RegisterUpdateEvent(WorldContextObject, updateDelegate);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		return updateDelegate.GetHandle();
	}

	UE_DEPRECATED(5.2, "Use UpdateCall instead.")
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Registerred update function will be called every frame from mainthread.", WorldContext = "WorldContextObject", DeprecatedFunction, DeprecationMessage = "Use UpdateCall instead."), Category = DreamWidgetTween)
	static FDreamWidgetTweenDelegateHandleWrapper RegisterUpdateEvent(UObject* WorldContextObject, const FDreamWidgetTweenerFloatDynamicDelegate& update)
	{
		FDreamWidgetTweenUpdateDelegate updateDelegate = FDreamWidgetTweenUpdateDelegate::CreateLambda([update](float deltaTime) { update.ExecuteIfBound(deltaTime); });
		FDreamWidgetTweenDelegateHandleWrapper delegateHandle(updateDelegate.GetHandle());
		// Disable deprecation warnings so we can call the deprecated function to support this function (which is also deprecated)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		UDreamWidgetTweenManager::RegisterUpdateEvent(WorldContextObject, updateDelegate);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		return delegateHandle;
	}

	UE_DEPRECATED(5.2, "Use UpdateCall instead of RegisterUpdateEvent, and KillIfIsTweening for returned tweener instead of this UnregisterUpdateEvent.")
	static void UnregisterUpdateEvent(UObject* WorldContextObject, const FDelegateHandle& handle)
	{
		// Disable deprecation warnings so we can call the deprecated function to support this function (which is also deprecated)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		UDreamWidgetTweenManager::UnregisterUpdateEvent(WorldContextObject, handle);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	}

	UE_DEPRECATED(5.2, "Use UpdateCall instead of RegisterUpdateEvent, and KillIfIsTweening for returned tweener instead of this UnregisterUpdateEvent.")
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Unregister the update function. \"delegateHandle\" is the return value when use RegisterUpdateEvent.", WorldContext = "WorldContextObject", DeprecatedFunction, DeprecationMessage = "Use UpdateCall instead of RegisterUpdateEvent, and KillIfIsTweening for returned tweener instead of this UnregisterUpdateEvent."), Category = DreamWidgetTween)
	static void UnregisterUpdateEvent(UObject* WorldContextObject, const FDreamWidgetTweenDelegateHandleWrapper& delegateHandle)
	{
		// Disable deprecation warnings so we can call the deprecated function to support this function (which is also deprecated)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		UDreamWidgetTweenManager::UnregisterUpdateEvent(WorldContextObject, delegateHandle.DelegateHandle);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	}

	/**
	 * Repeatedly call function.
	 * @param delayTime delay time before the first call
	 * @param interval interval time between every call
	 * @param repeatCount repeat count, -1 means infinite
	 * @return tweener
	 */
	static UDreamWidgetTweener* RepeatCall(UObject* WorldContextObject, const TFunction<void()>& callFunction, float delayTime, float interval, int repeatCount = 1)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, interval);
		if (Tweener)
		{
			Tweener
				->SetDelay(delayTime)
				->SetLoop(repeatCount == 1 || repeatCount == 0 ? EDreamWidgetTweenLoop::Once : EDreamWidgetTweenLoop::Restart, repeatCount)
				->OnCycleStart(callFunction);
		}
		return Tweener;
	}

	/**
	 * Repeatedly call function.
	 * @param delayTime delay time before the first call
	 * @param interval interval time between every call
	 * @param repeatCount repeat count, -1 means infinite
	 * \return tweener
	 */
	static UDreamWidgetTweener* RepeatCall(UObject* WorldContextObject, const FSimpleDelegate& callFunction, float delayTime, float interval, int repeatCount = 1)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, interval);
		if (Tweener)
		{
			Tweener
				->SetDelay(delayTime)
				->SetLoop(repeatCount == 1 || repeatCount == 0 ? EDreamWidgetTweenLoop::Once : EDreamWidgetTweenLoop::Restart, repeatCount)
				->OnCycleStart(callFunction);
		}
		return Tweener;
	}

	/**
	 * Repeatedly call function.
	 * @param delayTime delay time before the first call
	 * @param interval interval time between every call
	 * @param repeatCount repeat count, -1 means infinite
	 * @return tweener
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static UDreamWidgetTweener* RepeatCall(UObject* WorldContextObject, FDreamWidgetTweenerSimpleDynamicDelegate callFunction, float delayTime, float interval = 1.0f, int repeatCount = 1)
	{
		auto Tweener = UDreamWidgetTweenManager::VirtualTo(WorldContextObject, interval);
		if (Tweener)
		{
			Tweener
				->SetDelay(delayTime)
				->SetLoop(repeatCount == 1 || repeatCount == 0 ? EDreamWidgetTweenLoop::Once : EDreamWidgetTweenLoop::Restart, repeatCount)
				->OnCycleStart(callFunction);
		}
		return Tweener;
	}

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = DreamWidgetTween)
	static class UDreamWidgetTweenerSequence* CreateSequence(UObject* WorldContextObject)
	{
		return UDreamWidgetTweenManager::CreateSequence(WorldContextObject);
	}
};
