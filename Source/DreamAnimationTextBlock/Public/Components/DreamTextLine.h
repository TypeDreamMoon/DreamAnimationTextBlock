// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DreamTextChar.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextLine.generated.h"

class UWrapBox;
class UHorizontalBox;
class UDreamTextChar;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UDreamTextChar> CharWidgetClass = UDreamTextChar::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo Font = FSlateFontInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDreamTextBlockAnimationSetting* AnimationSetting = nullptr;
public:
	/**
	 * 推送字符
	 * @param Char 字符
	 * @return 创建的字符控件
	 */
	UFUNCTION(BlueprintCallable)
	UDreamTextChar* PushChar(FString Char);

	/**
	 * 设置字体
	 * @param InFont 新的字体
	 */
	UFUNCTION(BlueprintCallable)
	void SetFont(FSlateFontInfo InFont);

	/**
	 * 设置动画设置
	 * @param InAnimationSetting 动画设置
	 */
	UFUNCTION(BlueprintCallable)
	void SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting);
};
