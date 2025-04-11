// Copyright (c) 2022 Dream Moon. All Rights Reserved.


#include "Components/DreamTextLine.h"

#include "Components/WrapBox.h"


UDreamTextChar* UDreamTextLine::PushChar(FString Char, bool bInitializeAnimation)
{
	// 1. 检查CharWidgetClass是否有效
	if (CharWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("CharWidgetClass is null"));
		return nullptr;
	}

	// 2. 同步加载Widget类并检查有效性
	UClass* WidgetClass = CharWidgetClass.LoadSynchronous();
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CharWidgetClass"));
		return nullptr;
	}

	// 3. 创建CharWidget实例并检查有效性
	UDreamTextChar* CharWidget = CreateWidget<UDreamTextChar>(this, WidgetClass);
	if (!CharWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CharWidget"));
		return nullptr;
	}

	// 4. 检查HorizontalBox是否绑定
	if (!WrapBox)
	{
		UE_LOG(LogTemp, Error, TEXT("HorizontalBox is not bound"));
		return nullptr;
	}

	// 5. 调用SetChar前检查TextBlock是否有效（需在DreamTextChar.h中调整）
	if (!CharWidget->TextBlock)
	{
		UE_LOG(LogTemp, Error, TEXT("TextBlock is not bound in DreamTextChar"));
		return nullptr;
	}

	// 设置字符和字体
	CharWidget->SetChar(Char);
	CharWidget->SetFont(Font);
	CharWidget->SetAnimationSetting(AnimationSetting, bInitializeAnimation);

	// 6. 添加控件到WrapBoxSlot并检查Slot
	UWrapBoxSlot* WrapBoxSlot = WrapBox->AddChildToWrapBox(CharWidget);
	if (!WrapBoxSlot)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add CharWidget to HorizontalBox"));
		return nullptr;
	}

	return CharWidget;
}

void UDreamTextLine::SetFont(FSlateFontInfo InFont)
{
	Font = InFont;
}

void UDreamTextLine::SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting)
{
	AnimationSetting = InAnimationSetting;
}

