// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FString EnterText = "/";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UDreamTextLine> LineWidgetClass = UDreamTextLine::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EVerticalAlignment> LineVerticalAlignment = EVerticalAlignment::VAlign_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EHorizontalAlignment> LineHorizontalAlignment = EHorizontalAlignment::HAlign_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateChildSize LineSize = FSlateChildSize(ESlateSizeRule::Fill);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text = FText::FromString(TEXT("TextBlock"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo Font;

public:
	UFUNCTION(BlueprintCallable)
	void SetText(FText InText);

protected:
	virtual void NativePreConstruct() override;

	virtual void ClearProperty();
	virtual UDreamTextLine* CreateNewLine();

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UDreamTextChar*> TextChars = {};

	UPROPERTY(BlueprintReadOnly)
	UDreamTextLine* CurrentTextLine = nullptr;
};
