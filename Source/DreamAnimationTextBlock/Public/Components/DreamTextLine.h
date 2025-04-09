// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DreamTextChar.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextLine.generated.h"

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
	UHorizontalBox* HorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UDreamTextChar> CharWidgetClass = UDreamTextChar::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo Font;
public:
	UFUNCTION(BlueprintCallable)
	UDreamTextChar* PushChar(FString Char);

	UFUNCTION(BlueprintCallable)
	void SetFont(FSlateFontInfo InFont);
};
