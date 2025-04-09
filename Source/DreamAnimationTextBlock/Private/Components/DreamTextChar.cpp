// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DreamTextChar.h"

#include "DreamTextBlockAnimationSetting.h"
#include "LTweenBPLibrary.h"
#include "Components/TextBlock.h"

void UDreamTextChar::SetChar(FString InChar)
{
	Char = InChar;
	
	if (!TextBlock)
	{
		UE_LOG(LogTemp, Error, TEXT("TextBlock is null"));
		return;
	}
	
	TextBlock->SetText(FText::FromString(Char));
}

void UDreamTextChar::SetFont(FSlateFontInfo InFont)
{
	TextBlock->SetFont(InFont);
}

void UDreamTextChar::SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting)
{
	InitializeAnimation(InAnimationSetting);
}

void UDreamTextChar::InitializeAnimation(UDreamTextBlockAnimationSetting* InAnimationSetting)
{
	if (!InAnimationSetting) return;
	AnimationSetting = InAnimationSetting;

	TextBlock->SetRenderTransform(FWidgetTransform(
		AnimationSetting->Start.Translation,
		AnimationSetting->Start.Scale,
		AnimationSetting->Start.Shear,
		AnimationSetting->Start.Angle
		));
	TextBlock->SetRenderOpacity(AnimationSetting->Start.Opacity);
}

void UDreamTextChar::PlayAnimationWithCustomDuration(float AnimationDuration)
{
	if (!AnimationSetting) return;

	ULTweener* OpacityTweener = ULTweenBPLibrary::UMG_RenderOpacityTo(GWorld, TextBlock, AnimationSetting->End.Opacity, AnimationDuration, 0.f, AnimationSetting->Ease);
	ULTweener* TranslationTweener = ULTweenBPLibrary::UMG_RenderTransform_TranslationTo(GWorld, TextBlock, AnimationSetting->End.Translation, AnimationDuration, 0.f, AnimationSetting->Ease);
	ULTweener* ScaleTweener = ULTweenBPLibrary::UMG_RenderTransform_ScaleTo(GWorld, TextBlock, AnimationSetting->End.Scale, AnimationDuration, 0.f, AnimationSetting->Ease);
	ULTweener* ShearTweener = ULTweenBPLibrary::UMG_RenderTransform_ShearTo(GWorld, TextBlock, AnimationSetting->End.Shear, AnimationDuration, 0.f, AnimationSetting->Ease);
	ULTweener* AngleTweener = ULTweenBPLibrary::UMG_RenderTransform_AngleTo(GWorld, TextBlock, AnimationSetting->End.Angle, AnimationDuration, 0.f, AnimationSetting->Ease);
	if (AnimationSetting->Ease == ELTweenEase::CurveFloat)
	{
		OpacityTweener->SetCurveFloat(AnimationSetting->CurveFloat);
		TranslationTweener->SetCurveFloat(AnimationSetting->CurveFloat);
		ScaleTweener->SetCurveFloat(AnimationSetting->CurveFloat);
		ShearTweener->SetCurveFloat(AnimationSetting->CurveFloat);
		AngleTweener->SetCurveFloat(AnimationSetting->CurveFloat);
	}
}
