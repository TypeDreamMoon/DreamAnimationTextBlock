// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DreamAnimationTextBlockTools.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextLine.h"
#include "DreamTextBlock.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamTextBlock : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* VerticalBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UDreamTextLine> LineWidgetClass = UDreamTextLine::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EVerticalAlignment> LineVerticalAlignment = EVerticalAlignment::VAlign_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EHorizontalAlignment> LineHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateChildSize LineSize = FSlateChildSize(ESlateSizeRule::Fill);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MultiLine))
	FText Text = FText::FromString(TEXT("TextBlock"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo Font = FSlateFontInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimationDuration = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharAnimationDuration = 0.5f;

public:
	/**
	 * 设置文本
	 * @param InText 设置的文本
	 */
	UFUNCTION(BlueprintCallable)
	void SetText(FText InText);

	/**
	 * 播放动画从时长
	 * @param InSpeed 播放时长
	 */
	UFUNCTION(BlueprintCallable)
	void PlayAnimWithDuration(float InDuration);

	/**
	 * 播放动画
	 */
	UFUNCTION(BlueprintCallable)
	void PlayAnim();

	/**
	 * 停止动画
	 * @param bReset 是否重置动画
	 * @param bEndState 重置后是否为结束状态
	 */
	UFUNCTION(BlueprintCallable)
	void StopAnim(bool bReset = true, bool bEndState = true);

	/**
	 * 动画是否正在播放
	 * @return 动画是否正在播放
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimPlaying() const;

protected:
	virtual void NativePreConstruct() override;

	virtual void ClearProperty();
	virtual UDreamTextLine* CreateNewLine();

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextChar*> TextChars = {};

	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextLine*> Lines = {};

	UPROPERTY(BlueprintReadOnly)
	UDreamTextLine* CurrentTextLine = nullptr;

	FDreamAnimationTextBlockTools::FForEachWithDelayHandle AnimTimerHandle;
};
