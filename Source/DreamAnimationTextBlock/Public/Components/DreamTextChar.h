// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextChar.generated.h"

class ULTweener;
class ULTweenerSequence;
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
	 */
	UFUNCTION(BlueprintCallable)
	void SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting);

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
	 */
	UFUNCTION(BlueprintCallable)
	void StopAnim(bool bReset = true, bool bEndState = true);

	/**
	 * 当前动画是否正在播放
	 * @return 当前动画是否正在播放
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimPlaying() const { return bPlaying; }
public:
	UPROPERTY(BlueprintReadOnly)
	FString Char;

	UPROPERTY(BlueprintReadOnly)
	bool bPlaying = false;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ULTweener*> Tweeners;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerHandle;
};
