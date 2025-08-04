// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamAnimationTextBlockTools.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextChar.generated.h"

class UWrapBox;
class UDreamWidgetTweener;
class UDreamWidgetTweenerSequence;
class UDreamTextBlockAnimationSetting;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamTextChar : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* TextBlock = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DreamTextChar|Animation")
    UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter="SetTextPadding", Getter="GetTextPadding", Category="DreamTextChar|Layout")
    FVector2D TextPadding = FVector2D(0.f, 0.f);

public:
    /**
     * 设置字符
     * @param InChar 设置的字符
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Text")
    void SetChar(FString InChar);

    /**
     * 设置字体
     * @param InFont 设置的字体
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Text")
    void SetFont(FSlateFontInfo InFont);

    /**
     * 设置动画设置
     * @param InAnimationSetting 新的动画设置
     * @param bInitialize 是否初始化动画
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Animation")
    void SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bInitialize = true);

    /**
     * 初始化动画
     * @param InAnimationSetting 初始化的动画设置
     * @param bEndState 是否初始化为结束状态
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Animation")
    void InitializeAnimation(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bEndState = false);

    /**
     * 播放动画(自定义动画长度)
     * @param AnimationDuration 字符动画时长
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Animation")
    void PlayAnimationWithCustomDuration(float AnimationDuration);

    /**
     * 停止动画
     * @param bReset 重置动画
     * @param bEndState 是否重置为结束状态
     * @param bCallCompleted 是否调用动画完成事件
     */
    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Animation")
    void StopAnim(bool bReset = true, bool bEndState = true, bool bCallCompleted = false);

    /**
     * 当前动画是否正在播放
     * @return 当前动画是否正在播放
     */
    UFUNCTION(BlueprintPure, Category="DreamTextChar|Animation")
    bool IsAnimPlaying() const { return bPlaying; }

    UFUNCTION(BlueprintCallable, Category="DreamTextChar|Layout")
    void SetTextPadding(FVector2D NewPadding);

    UFUNCTION(BlueprintPure, Category="DreamTextChar|Layout")
    FVector2D GetTextPadding() const;

    FSimpleDelegate OnAnimationCompletedCpp;

public:
    UPROPERTY(BlueprintReadOnly, Category="DreamTextChar|State")
    FString Char;

    UPROPERTY(BlueprintReadOnly, Category="DreamTextChar|State")
    bool bPlaying = false;

    UPROPERTY(BlueprintReadOnly, Category="DreamTextChar|State")
    TArray<UDreamWidgetTweener*> Tweeners;

    UPROPERTY(BlueprintReadOnly, Category="DreamTextChar|State")
    FTimerHandle TimerHandle;
};
