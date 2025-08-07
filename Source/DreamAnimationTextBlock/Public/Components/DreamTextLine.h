// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamTextChar.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextLine.generated.h"

class UWrapBox;
class UDreamTextChar;
class UDreamTextBlockAnimationSetting;

/**
 * 
 */
UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamTextLine : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(Meta = (BindWidget))
    UWrapBox* WrapBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DreamTextLine|Class")
    TSoftClassPtr<UDreamTextChar> CharWidgetClass = UDreamTextChar::StaticClass();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DreamTextLine|Appearance")
    FSlateFontInfo Font = FSlateFontInfo();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DreamTextLine|Animation")
    UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;

public:
    /**
     * 推送字符
     * @param Char 字符
     * @param bInitializeAnimation 是否初始化动画
     * @return 创建的字符控件
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextLine|Actions")
    UDreamTextChar* PushChar(FString Char, bool bInitializeAnimation = false);

    /**
     * 设置字体
     * @param InFont 新的字体
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextLine|Appearance")
    void SetFont(FSlateFontInfo InFont);

    /**
     * 设置动画设置
     * @param InAnimationSetting 动画设置
     * @param bInitializeAnimation 是否初始化动画
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextLine|Animation")
    void SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bInitializeAnimation = true);

protected:
    TArray<TWeakObjectPtr<UDreamTextChar>> Chars;
};
