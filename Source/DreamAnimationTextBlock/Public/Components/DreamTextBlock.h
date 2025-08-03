// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamAnimationTextBlockTools.h"
#include "DreamAnimationTextBlockTypes.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextLine.h"
#include "DreamTextBlock.generated.h"

enum class EDreamTextAnimationPlayType : uint8;
class UVerticalBox;

/**
 * UDreamTextBlock 是一个用于显示带动画效果文本的用户控件。
 * 支持按字符播放动画、设置字体、对齐方式、自动换行等功能。
 */
UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamTextBlock : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 垂直布局容器，用于放置每一行文本。
	 */
	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* VerticalBox = nullptr;

	/**
	 * 每一行文本使用的控件类。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock")
	TSoftClassPtr<UDreamTextLine> LineWidgetClass = UDreamTextLine::StaticClass();

	/**
	 * 每行文本的垂直对齐方式。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	TEnumAsByte<EVerticalAlignment> LineVerticalAlignment = EVerticalAlignment::VAlign_Center;

	/**
	 * 每行文本的水平对齐方式。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	TEnumAsByte<EHorizontalAlignment> LineHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/**
	 * 每行文本的尺寸规则。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	FSlateChildSize LineSize = FSlateChildSize(ESlateSizeRule::Fill);

	/**
	 * 显示的文本内容。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text", Meta = (MultiLine))
	FText Text = FText::FromString(TEXT("TextBlock"));

	/**
	 * 文本的内边距。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	FVector2D TextPadding = FVector2D(0.f, 0.f);

	/**
	 * 文本使用的字体信息。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Text")
	FSlateFontInfo Font = FSlateFontInfo();

	/**
	 * 动画设置对象。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;

	/**
	 * 整体动画播放时长。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	float AnimationDuration = 5.f;

	/**
	 * 每个字符动画的播放时长。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	float CharAnimationDuration = 0.2f;

	/**
	 * 每行之间的动画延迟时间。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	float LineAnimationDelay = 0.1f;

	/**
	 * 是否在构造时自动播放动画。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	bool bAutoPlay = false;

	/**
	 * 自动播放时的动画播放类型。
	 * 仅在 bAutoPlay 为 true 时生效。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation", Meta = (EditCondition="bAutoPlay", EditConditionHides))
	EDreamTextAnimationPlayType PlayType = EDreamTextAnimationPlayType::Sequentially;

	/**
	 * 是否在初始化时预处理动画数据。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Animation")
	bool bPreInitialize = true;

	/**
	 * 是否启用显式换行宽度。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Wrap")
	bool bExplicitWrapSize = false;

	/**
	 * 显式换行宽度值。
	 * 仅在 bExplicitWrapSize 为 true 时生效。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextBlock|Wrap", Meta = (ClampMin = "0.0", EditConditionHides, EditCondition = "bExplicitWrapSize"))
	float WrapSize = 500.f;

public:
	/**
	 * 设置要显示的文本内容。
	 * @param InText 要设置的文本
	 */
	UFUNCTION(BlueprintCallable)
	void SetText(FText InText);

	/**
	 * 根据指定时长播放动画。
	 * @param InDuration 动画播放时长
	 * @param InPlayType 动画播放类型
	 */
	UFUNCTION(BlueprintCallable)
	void PlayAnimWithDuration(float InDuration, EDreamTextAnimationPlayType InPlayType = EDreamTextAnimationPlayType::Sequentially);

	/**
	 * 播放动画。
	 * @param InPlayType 动画播放类型
	 */
	UFUNCTION(BlueprintCallable)
	void PlayAnim(EDreamTextAnimationPlayType InPlayType = EDreamTextAnimationPlayType::Sequentially);

	/**
	 * 停止当前动画。
	 * @param bReset 是否重置动画状态
	 * @param bEndState 重置后是否处于结束状态
	 * @param bCallCompleted 是否触发完成回调
	 */
	UFUNCTION(BlueprintCallable)
	void StopAnim(bool bReset = true, bool bEndState = true, bool bCallCompleted = false);

	/**
	 * 判断动画是否正在播放。
	 * @return 如果动画正在播放则返回 true，否则返回 false
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimPlaying() const;

	/**
	 * 获取当前文本的字符数量。
	 * @return 当前文本的字符数量
	 */
	UFUNCTION(BlueprintPure)
	int32 GetTextNum() const { return TextChars.Num(); }

	/**
	 * 获取当前显示的文本内容。
	 * @return 当前文本内容
	 */
	UFUNCTION(BlueprintPure)
	FText GetText() const { return Text; }

	/**
	 * 获取当前使用的字体信息。
	 * @return 当前字体信息
	 */
	UFUNCTION(BlueprintPure)
	FSlateFontInfo GetFont() const { return Font; }

	/**
	 * 初始化动画相关数据。
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeAnimation();

	DECLARE_DYNAMIC_DELEGATE(FDreamTextBlockSimpleDelegate);

	/**
	 * 注册最后一个字符动画完成时的回调。
	 * @param Delegate 回调函数
	 */
	UFUNCTION(BlueprintCallable)
	void OnLastCharAnimationCompleted(const FDreamTextBlockSimpleDelegate& Delegate);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDreamTextBlockCharDelegate, UDreamTextChar*, Char);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDreamTextBlockDelegate);

	/**
	 * 字符动画开始播放时触发的事件。
	 */
	UPROPERTY(BlueprintAssignable)
	FDreamTextBlockCharDelegate OnCharAnimPlay;

	/**
	 * 所有动画播放完成时触发的事件。
	 */
	UPROPERTY(BlueprintAssignable)
	FDreamTextBlockDelegate OnAnimCompleted;

protected:
	/**
	 * 在蓝图预构建阶段调用，用于初始化控件属性。
	 */
	virtual void NativePreConstruct() override;

	/**
	 * 在控件构造完成后调用，用于执行初始化逻辑。
	 */
	virtual void NativeConstruct() override;

	/**
	 * 清理控件属性。
	 */
	virtual void ClearProperty();

	/**
	 * 创建一个新的文本行控件。
	 * @return 新创建的文本行控件
	 */
	virtual UDreamTextLine* CreateNewLine();

	/**
	 * 内部设置文本内容的方法。
	 * @param InText 要设置的文本
	 * @param bIsPreConstruct 是否处于预构建阶段
	 */
	virtual void Internal_SetText(FText InText, bool bIsPreConstruct);

protected:
	/**
	 * 所有字符控件的数组。
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextChar*> TextChars = {};

	/**
	 * 所有文本行控件的数组。
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextLine*> TextLines = {};

	/**
	 * 当前正在处理的文本行控件。
	 */
	UPROPERTY(BlueprintReadOnly)
	UDreamTextLine* CurrentTextLine = nullptr;

	/**
	 * 动画播放的标识符，用于防止重复播放。
	 */
	int32 AnimPlayToken = 0;

	/**
	 * 随机动画使用的定时器句柄数组。
	 */
	TArray<FTimerHandle> RandomAnimTimerHandles;

	/**
	 * 动画播放使用的延迟循环句柄。
	 */
	FDreamAnimationTextBlockTools::FForEachWithDelayHandle AnimTimerHandle;
};
