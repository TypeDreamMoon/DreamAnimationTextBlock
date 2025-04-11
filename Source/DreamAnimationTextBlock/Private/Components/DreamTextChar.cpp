// Copyright (c) 2022 Dream Moon. All Rights Reserved.


#include "Components/DreamTextChar.h"

#include "DreamTextBlockAnimationSetting.h"
#include "LTweenBPLibrary.h"
#include "LTweenerSequence.h"
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

void UDreamTextChar::SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bInitialize)
{
	if (bInitialize)
	{
		InitializeAnimation(InAnimationSetting);
	}
	else
	{
		AnimationSetting = InAnimationSetting;
	}
}

void UDreamTextChar::InitializeAnimation(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bEndState)
{
	if (!InAnimationSetting)
	{
		return;
	}
	
	AnimationSetting = InAnimationSetting;
	bPlaying = false;

	TextBlock->SetRenderTransform(FWidgetTransform(
		bEndState ? AnimationSetting->End.Translation : AnimationSetting->Start.Translation,
		bEndState ? AnimationSetting->End.Scale : AnimationSetting->Start.Scale,
		bEndState ? AnimationSetting->End.Shear : AnimationSetting->Start.Shear,
		bEndState ? AnimationSetting->End.Angle : AnimationSetting->Start.Angle
	));

	TextBlock->SetRenderOpacity(bEndState ? AnimationSetting->End.Opacity : AnimationSetting->Start.Opacity);
}

void UDreamTextChar::PlayAnimationWithCustomDuration(float AnimationDuration)
{
	if (!AnimationSetting) return;

	if (bPlaying)
	{
		StopAnim(true);
	}

	InitializeAnimation(AnimationSetting, false);

	Tweeners.Add(ULTweenBPLibrary::UMG_RenderOpacityTo(GWorld, TextBlock, AnimationSetting->End.Opacity, AnimationDuration, 0.f, AnimationSetting->Ease));
	Tweeners.Add(ULTweenBPLibrary::UMG_RenderTransform_TranslationTo(GWorld, TextBlock, AnimationSetting->End.Translation, AnimationDuration, 0.f, AnimationSetting->Ease));
	Tweeners.Add(ULTweenBPLibrary::UMG_RenderTransform_ScaleTo(GWorld, TextBlock, AnimationSetting->End.Scale, AnimationDuration, 0.f, AnimationSetting->Ease));
	Tweeners.Add(ULTweenBPLibrary::UMG_RenderTransform_ShearTo(GWorld, TextBlock, AnimationSetting->End.Shear, AnimationDuration, 0.f, AnimationSetting->Ease));
	Tweeners.Add(ULTweenBPLibrary::UMG_RenderTransform_AngleTo(GWorld, TextBlock, AnimationSetting->End.Angle, AnimationDuration, 0.f, AnimationSetting->Ease));
	
	GWorld->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		Tweeners.Empty();
		OnAnimationCompletedCpp.ExecuteIfBound();
	}, AnimationDuration, false);
	
	if (AnimationSetting->Ease == ELTweenEase::CurveFloat)
	{
		for (auto Element : Tweeners)
		{
			Element->SetCurveFloat(AnimationSetting->CurveFloat);
		}
	}

	bPlaying = true;
}

void UDreamTextChar::StopAnim(bool bReset, bool bEndState, bool bCallCompleted)
{
	if (bPlaying)
	{
		for (auto Element : Tweeners)
		{
			Element->Kill();
		}

		if (TimerHandle.IsValid())
		{
			GWorld->GetTimerManager().ClearTimer(TimerHandle);
		}

		Tweeners.Empty();
	}

	if (bReset)
		InitializeAnimation(AnimationSetting, bEndState);

	if (bCallCompleted)
		OnAnimationCompletedCpp.ExecuteIfBound();
}
