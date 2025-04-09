// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DreamTextLine.h"

#include "Components/HorizontalBox.h"


UDreamTextChar* UDreamTextLine::PushChar(FString Char)
{
	UDreamTextChar* CharWidget = CreateWidget<UDreamTextChar>(this, CharWidgetClass.LoadSynchronous());
	CharWidget->SetChar(Char);
	CharWidget->SetFont(Font);
	HorizontalBox->AddChildToHorizontalBox(CharWidget);
	return CharWidget;
}

void UDreamTextLine::SetFont(FSlateFontInfo InFont)
{
	Font = InFont;
}
