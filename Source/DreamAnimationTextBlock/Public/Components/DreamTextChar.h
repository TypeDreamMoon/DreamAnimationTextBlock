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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter="SetTextPadding", Getter="GetTextPadding")
	FVector2D TextPadding = FVector2D(0.f, 0.f);

public:
	/**
	 * 设置字符
	 * @param InChar 设置的字符
	 */
	UFUNCTION(BlueprintCallable)
	void SetChar(FString InChar);

	/**
	 * 设置字体
	 * @param InFont 设置的字体
	 */
	UFUNCTION(BlueprintCallable)
	void SetFont(FSlateFontInfo InFont);

	/**
	 * 设置动画设置
	 * @param InAnimationSetting 新的动画设置
	 * @param bInitialize 是否初始化动画
	 */
	UFUNCTION(BlueprintCallable)
	void SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bInitialize = true);

	/**
	 * 初始化动画
	 * @param InAnimationSetting 初始化的动画设置
	 * @param bEndState 是否初始化为结束状态
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeAnimation(UDreamTextBlockAnimationSetting* InAnimationSetting, bool bEndState = false);

	/**
	 * 播放动画(自定义动画长度)
	 * @param AnimationDuration 字符动画时长
	 */
	UFUNCTION(BlueprintCallable)
	void PlayAnimationWithCustomDuration(float AnimationDuration);

	/**
	 * 停止动画
	 * @param bReset 重置动画
	 * @param bEndState 是否重置为结束状态
	 * @param bCallCompleted 是否调用动画完成事件
	 */
	UFUNCTION(BlueprintCallable)
	void StopAnim(bool bReset = true, bool bEndState = true, bool bCallCompleted = false);

	/**
	 * 当前动画是否正在播放
	 * @return 当前动画是否正在播放
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimPlaying() const { return bPlaying; }

	UFUNCTION(BlueprintCallable)
	void SetTextPadding(FVector2D NewPadding);

	UFUNCTION(BlueprintPure)
	FVector2D GetTextPadding() const;

	FSimpleDelegate OnAnimationCompletedCpp;

public:
	UPROPERTY(BlueprintReadOnly)
	FString Char;

	UPROPERTY(BlueprintReadOnly)
	bool bPlaying = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamWidgetTweener*> Tweeners;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerHandle;
};
