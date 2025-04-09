// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DreamTextBlock.h"

#include "DreamAnimationTextBlockTools.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/KismetStringLibrary.h"

void UDreamTextBlock::SetText(FText InText)
{
	Text = InText;
	FString StrText = Text.ToString();
	float LineWidth = 0.f;

	ClearProperty();

	auto Push = [this](const FString& InChar)
	{
		UDreamTextChar* CharWidget = CurrentTextLine->PushChar(InChar);
		return CharWidget;
	};


	for (auto Element : UKismetStringLibrary::GetCharacterArrayFromString(StrText))
	{
		if (Element == EnterText)
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

void UDreamTextBlock::PlayAnim()
{
	FDreamAnimationTextBlockTools::ForEachWithDelay<decltype(TextChars), UDreamTextChar*>(
		GWorld, TextChars, AnimationDuration / TextChars.Num(), [this](UDreamTextChar* Element)
	{
		Element->PlayAnimationWithCustomDuration(AnimationDuration);
	});
}

void UDreamTextBlock::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetText(Text);
}

void UDreamTextBlock::ClearProperty()
{
	TextChars.Empty();
	VerticalBox->ClearChildren();
	CurrentTextLine = nullptr;
}

UDreamTextLine* UDreamTextBlock::CreateNewLine()
{
	UDreamTextLine* LineWidget = CreateWidget<UDreamTextLine>(this, LineWidgetClass.LoadSynchronous());

	if (!LineWidget || !VerticalBox) return nullptr;

	LineWidget->SetFont(Font);
	LineWidget->SetAnimationSetting(AnimationSetting);
	
	UVerticalBoxSlot* VBSlot = VerticalBox->AddChildToVerticalBox(LineWidget);
	VBSlot->SetVerticalAlignment(LineVerticalAlignment);
	VBSlot->SetHorizontalAlignment(LineHorizontalAlignment);
	VBSlot->SetSize(LineSize);
	return LineWidget;
}
