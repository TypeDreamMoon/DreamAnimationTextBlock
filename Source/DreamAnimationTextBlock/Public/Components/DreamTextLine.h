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
	FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDreamTextBlockAnimationSetting* AnimationSetting;
public:
	UFUNCTION(BlueprintCallable)
	UDreamTextChar* PushChar(FString Char);

	UFUNCTION(BlueprintCallable)
	void SetFont(FSlateFontInfo InFont);

	UFUNCTION(BlueprintCallable)
	void SetAnimationSetting(UDreamTextBlockAnimationSetting* InAnimationSetting);
};
