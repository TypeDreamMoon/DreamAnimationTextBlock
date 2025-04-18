// Copyright (c) 2022 Dream Moon. All Rights Reserved.

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock")
	TSoftClassPtr<UDreamTextLine> LineWidgetClass = UDreamTextLine::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	TEnumAsByte<EVerticalAlignment> LineVerticalAlignment = EVerticalAlignment::VAlign_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	TEnumAsByte<EHorizontalAlignment> LineHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	FSlateChildSize LineSize = FSlateChildSize(ESlateSizeRule::Fill);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text", Meta = (MultiLine))
	FText Text = FText::FromString(TEXT("TextBlock"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	FVector2D TextPadding = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	FSlateFontInfo Font = FSlateFontInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	float AnimationDuration = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Wrap")
	bool bExplicitWrapSize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Wrap", Meta = (ClampMin = "0.0", EditConditionHides, EditCondition = "bExplicitWrapSize"))
	float WrapSize = 500.f;

public:
	/**
	 * 设置文本
	 * @param InText 设置的文本
	 */
	UFUNCTION(BlueprintCallable)
	void SetText(FText InText);

	/**
	 * 播放动画从时长
	 * @param InDuration 播放时长
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
	 * @param bCallCompleted 是否调用回调
	 */
	UFUNCTION(BlueprintCallable)
	void StopAnim(bool bReset = true, bool bEndState = true, bool bCallCompleted = false);

	/**
	 * 动画是否正在播放
	 * @return 动画是否正在播放
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimPlaying() const;

	/**
	 * 获取文本字符个数
	 * @return 字符个数
	 */
	UFUNCTION(BlueprintPure)
	int32 GetTextNum() const { return TextChars.Num(); }

	/**
	 * 获取文本
	 * @return 文本
	 */
	UFUNCTION(BlueprintPure)
	FText GetText() const { return Text; }

	/**
	 * 获取字体
	 * @return 字体
	 */
	UFUNCTION(BlueprintPure)
	FSlateFontInfo GetFont() const { return Font; }

	DECLARE_DYNAMIC_DELEGATE(FDreamTextBlockSimpleDelegate);

	/**
	 * 最后一个字符动画结束回调
	 * @param Delegate 回调
	 */
	UFUNCTION(BlueprintCallable)
	void OnLastCharAnimationCompleted(const FDreamTextBlockSimpleDelegate& Delegate);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDreamTextBlockCharDelegate, UDreamTextChar*, Char);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDreamTextBlockDelegate);

	/**
	 * 字符动画播放时
	 */
	UPROPERTY(BlueprintAssignable)
	FDreamTextBlockCharDelegate OnCharAnimPlay;

	/**
	 * 动画播放结束时
	 */
	UPROPERTY(BlueprintAssignable)
	FDreamTextBlockDelegate OnAnimCompleted;

protected:
	virtual void NativePreConstruct() override;

	virtual void ClearProperty();
	virtual UDreamTextLine* CreateNewLine();
	virtual void Internal_SetText(FText InText, bool bIsPreConstruct);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextChar*> TextChars = {};

	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextLine*> Lines = {};

	UPROPERTY(BlueprintReadOnly)
	UDreamTextLine* CurrentTextLine = nullptr;

	FDreamAnimationTextBlockTools::FForEachWithDelayHandle AnimTimerHandle;
};
