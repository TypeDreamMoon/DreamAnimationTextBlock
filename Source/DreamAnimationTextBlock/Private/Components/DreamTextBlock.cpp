// Copyright (c) 2022 Dream Moon. All Rights Reserved.


#include "Components/DreamTextBlock.h"

#include "DreamAnimationTextBlockTools.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Kismet/KismetStringLibrary.h"

void UDreamTextBlock::SetText(FText InText)
{
	Internal_SetText(InText, false);
}

void UDreamTextBlock::PlayAnimWithDuration(float InDuration)
{
	AnimTimerHandle = FDreamAnimationTextBlockTools::ForEachWithDelay<decltype(TextChars), UDreamTextChar*>(
		GWorld, TextChars, InDuration, [this](UDreamTextChar* Element)
		{
			Element->PlayAnimationWithCustomDuration(AnimationDuration);
			OnCharAnimPlay.Broadcast(Element);
		},[this]()
		{
			OnAnimCompleted.Broadcast();
		});
}

void UDreamTextBlock::PlayAnim()
{
	PlayAnimWithDuration(AnimationDuration / TextChars.Num());
}

void UDreamTextBlock::StopAnim(bool bReset, bool bEndState, bool bCallCompleted)
{
	if (IsAnimPlaying())
	{
		AnimTimerHandle.Stop();
	}

	for (auto Element : TextChars)
	{
		Element->StopAnim(bReset, bEndState, bCallCompleted);
	}
}

bool UDreamTextBlock::IsAnimPlaying() const
{
	for (auto Element : TextChars)
	{
		if (Element->IsAnimPlaying())
			return true;
	}

	return false;
}

void UDreamTextBlock::OnLastCharAnimationCompleted(const FDreamTextBlockSimpleDelegate& Delegate)
{
	TextChars.Last()->OnAnimationCompletedCpp.BindLambda([Delegate]()
	{
		Delegate.ExecuteIfBound();
	});
}

void UDreamTextBlock::NativePreConstruct()
{
	Super::NativePreConstruct();

	Internal_SetText(Text, true);
}

void UDreamTextBlock::ClearProperty()
{
	TextChars.Empty();
	Lines.Empty();
	VerticalBox->ClearChildren();
	CurrentTextLine = nullptr;
}

UDreamTextLine* UDreamTextBlock::CreateNewLine()
{
	// Create LineWidget
	UDreamTextLine* LineWidget = CreateWidget<UDreamTextLine>(this, LineWidgetClass.LoadSynchronous());
	Lines.Add(LineWidget);

	if (!LineWidget || !VerticalBox || !LineWidget->WrapBox) return nullptr;

	// Set LineWidget Properties
	LineWidget->SetFont(Font);
	LineWidget->SetAnimationSetting(AnimationSetting);
	LineWidget->WrapBox->SetExplicitWrapSize(bExplicitWrapSize);
	LineWidget->WrapBox->SetWrapSize(WrapSize);
	LineWidget->WrapBox->SetInnerSlotPadding(TextPadding);

	// Add LineWidget to VerticalBox
	UVerticalBoxSlot* VBSlot = VerticalBox->AddChildToVerticalBox(LineWidget);
	VBSlot->SetVerticalAlignment(LineVerticalAlignment);
	VBSlot->SetHorizontalAlignment(LineHorizontalAlignment);
	VBSlot->SetSize(LineSize);

	return LineWidget;
}

void UDreamTextBlock::Internal_SetText(FText InText, bool bIsPreConstruct)
{
	Text = InText;
	FString StrText = Text.ToString();

	ClearProperty();

	auto Push = [this, bIsPreConstruct](const FString& InChar)
	{
		UDreamTextChar* CharWidget = CurrentTextLine->PushChar(InChar, !bIsPreConstruct);
		return CharWidget;
	};


	for (auto Element : UKismetStringLibrary::GetCharacterArrayFromString(StrText))
	{
		if (Element == "\n")
		{
			CurrentTextLine = CreateNewLine();
		}
		else
		{
			if (CurrentTextLine)
			{
				if (UDreamTextChar* Char = Push(Element))
				{
					TextChars.Add(Char);
				}
			}
			else
			{
				CurrentTextLine = CreateNewLine();
				if (UDreamTextChar* Char = Push(Element))
					TextChars.Add(Char);
			}
		}
	}
}
