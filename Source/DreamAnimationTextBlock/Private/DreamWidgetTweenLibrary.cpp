// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#include "DreamWidgetTweenLibrary.h"
#include "DreamWidgetTweener.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/MeshComponent.h"

#pragma region UMG

#include "Components/Widget.h"

UDreamWidgetTweener* UDreamWidgetTweenLibrary::UMG_RenderTransform_TranslationTo(UObject* WorldContextObject, class UWidget* target, const FVector2D& endValue, float duration, float delay, EDreamWidgetTweenEase ease)
{
	if (!IsValid(target))
	{
		return nullptr;
	}
	auto Tweener = UDreamWidgetTweenManager::To(WorldContextObject, FDreamWidgetTweenVector2DGetterFunction::CreateWeakLambda(target, [=]
	                                            {
		                                            return target->GetRenderTransform().Translation;
	                                            }), FDreamWidgetTweenVector2DSetterFunction::CreateUObject(target, &UWidget::SetRenderTranslation)
	                                            , endValue, duration);
	if (Tweener)
	{
		Tweener->SetDelay(delay)->SetEase(ease)->SetAffectByGamePause(false)->SetAffectByTimeDilation(false);
	}
	return Tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenLibrary::UMG_RenderTransform_AngleTo(UObject* WorldContextObject, UWidget* target, float endValue, float duration, float delay, EDreamWidgetTweenEase ease)
{
	if (!IsValid(target))
	{
		return nullptr;
	}
	auto Tweener = UDreamWidgetTweenManager::To(WorldContextObject
	                                            , FDreamWidgetTweenFloatGetterFunction::CreateUObject(target, &UWidget::GetRenderTransformAngle)
	                                            , FDreamWidgetTweenFloatSetterFunction::CreateUObject(target, &UWidget::SetRenderTransformAngle)
	                                            , endValue, duration);
	if (Tweener)
	{
		Tweener->SetDelay(delay)->SetEase(ease)->SetAffectByGamePause(false)->SetAffectByTimeDilation(false);
	}
	return Tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenLibrary::UMG_RenderTransform_ScaleTo(UObject* WorldContextObject, UWidget* target, const FVector2D& endValue, float duration, float delay, EDreamWidgetTweenEase ease)
{
	if (!IsValid(target))
	{
		return nullptr;
	}
	auto Tweener = UDreamWidgetTweenManager::To(WorldContextObject, FDreamWidgetTweenVector2DGetterFunction::CreateWeakLambda(target, [=]
	                                            {
		                                            return target->GetRenderTransform().Scale;
	                                            }), FDreamWidgetTweenVector2DSetterFunction::CreateUObject(target, &UWidget::SetRenderScale)
	                                            , endValue, duration);
	if (Tweener)
	{
		Tweener->SetDelay(delay)->SetEase(ease)->SetAffectByGamePause(false)->SetAffectByTimeDilation(false);
	}
	return Tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenLibrary::UMG_RenderTransform_ShearTo(UObject* WorldContextObject, UWidget* target, const FVector2D& endValue, float duration, float delay, EDreamWidgetTweenEase ease)
{
	if (!IsValid(target))
	{
		return nullptr;
	}
	auto Tweener = UDreamWidgetTweenManager::To(WorldContextObject, FDreamWidgetTweenVector2DGetterFunction::CreateWeakLambda(target, [=]
	                                            {
		                                            return target->GetRenderTransform().Shear;
	                                            }), FDreamWidgetTweenVector2DSetterFunction::CreateUObject(target, &UWidget::SetRenderShear)
	                                            , endValue, duration);
	if (Tweener)
	{
		Tweener->SetDelay(delay)->SetEase(ease)->SetAffectByGamePause(false)->SetAffectByTimeDilation(false);
	}
	return Tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenLibrary::UMG_RenderOpacityTo(UObject* WorldContextObject, UWidget* target, float endValue, float duration, float delay, EDreamWidgetTweenEase ease)
{
	if (!IsValid(target))
	{
		return nullptr;
	}
	auto Tweener = UDreamWidgetTweenManager::To(WorldContextObject
	                                            , FDreamWidgetTweenFloatGetterFunction::CreateUObject(target, &UWidget::GetRenderOpacity)
	                                            , FDreamWidgetTweenFloatSetterFunction::CreateUObject(target, &UWidget::SetRenderOpacity)
	                                            , endValue, duration);
	if (Tweener)
	{
		Tweener->SetDelay(delay)->SetEase(ease)->SetAffectByGamePause(false)->SetAffectByTimeDilation(false);
	}
	return Tweener;
}

#include "Components/DreamTextChar.h"

UDreamWidgetTweener* UDreamWidgetTweenLibrary::UMG_DreamAnimTextCharPaddingTo(UObject* WorldContextObject, class UDreamTextChar* target, const FVector2D& endValue, float duration, float delay, EDreamWidgetTweenEase ease)
{
	if (!IsValid(target))
	{
		return nullptr;
	}
	auto Tweener = UDreamWidgetTweenManager::To(WorldContextObject
	                                            , FDreamWidgetTweenVector2DGetterFunction::CreateUObject(target, &UDreamTextChar::GetTextPadding)
	                                            , FDreamWidgetTweenVector2DSetterFunction::CreateUObject(target, &UDreamTextChar::SetTextPadding)
	                                            , endValue, duration);
	if (Tweener)
	{
		Tweener->SetDelay(delay)->SetEase(ease)->SetAffectByGamePause(false)->SetAffectByTimeDilation(false);
	}
	return Tweener;
}
#pragma endregion
