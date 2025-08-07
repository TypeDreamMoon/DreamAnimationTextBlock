// Copyright (c) 2022 Dream Moon. All Rights Reserved.


#include "Components/DreamTextLine.h"

#include "Components/WrapBox.h"


UDreamTextChar* UDreamTextLine::PushChar(FString Char, bool bInitializeAnimation)
{
	if (CharWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("CharWidgetClass is null"));
		return nullptr;
	}

	UClass* WidgetClass = CharWidgetClass.LoadSynchronous();
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CharWidgetClass"));
		return nullptr;
	}

	UDreamTextChar* CharWidget = CreateWidget<UDreamTextChar>(this, WidgetClass);
	if (!CharWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CharWidget"));
		return nullptr;
	}

	if (!WrapBox)
	{
		UE_LOG(LogTemp, Error, TEXT("HorizontalBox is not bound"));
		return nullptr;
	}

	// Add to WrapBox first so Slot is assigned
	UWrapBoxSlot* WrapBoxSlot = WrapBox->AddChildToWrapBox(CharWidget);
	if (!WrapBoxSlot)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add CharWidget to WrapBox"));
		return nullptr;
	}

	if (!CharWidget->TextBlock)
	{
		UE_LOG(LogTemp, Error, TEXT("TextBlock is not bound in DreamTextChar"));
		return nullptr;
	}

	// 设置字符和字体
	CharWidget->SetChar(Char);
	CharWidget->SetFont(Font);
	CharWidget->SetAnimationSetting(AnimationSetting, bInitializeAnimation);

	Chars.Add(MakeWeakObjectPtr(CharWidget));

	return CharWidget;
}

void UDreamTextLine::SetFont(FSlateFontInfo InFont)
{
	Font = InFont;
}

void UDreamTextLine::SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bInitializeAnimation)
{
	AnimationSetting = InAnimationSetting;
	for (auto CharWidget : Chars)
	{
		if (CharWidget.IsValid())
		{
			CharWidget->SetAnimationSetting(AnimationSetting, bInitializeAnimation);
		}
	}
}
