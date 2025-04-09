// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DreamTextBlock.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/KismetStringLibrary.h"

void UDreamTextBlock::SetText(FText InText)
{
	Text = InText;
	FString StrText = Text.ToString(); 

	auto Push = [this](const FString& InChar)
	{
		UDreamTextChar* CharWidget = CurrentTextLine->PushChar(InChar);
		return CharWidget;
	};

	
	for (auto Element : UKismetStringLibrary::GetCharacterArrayFromString(StrText))
	{
		if (Element == EnterText)
		{
			CurrentTextLine = CreateNewLine();
		}
		else
		{
			if (CurrentTextLine)
			{
				TextChars.Add(Push(Element));
			}
			else
			{
				CreateNewLine();
				TextChars.Add(Push(Element));
			}
		}
	}
}

void UDreamTextBlock::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetText(Text);
}

void UDreamTextBlock::ClearProperty()
{
	TextChars.Empty();
	VerticalBox->ClearChildren();
	CurrentTextLine = nullptr;
}

UDreamTextLine* UDreamTextBlock::CreateNewLine()
{
	UDreamTextLine* LineWidget = CreateWidget<UDreamTextLine>(this, LineWidgetClass.LoadSynchronous());
	LineWidget->SetFont(Font);
	UVerticalBoxSlot* VBSlot = VerticalBox->AddChildToVerticalBox(LineWidget);
	VBSlot->SetVerticalAlignment(LineVerticalAlignment);
	VBSlot->SetHorizontalAlignment(LineHorizontalAlignment);
	VBSlot->SetSize(LineSize);
	return LineWidget;
}
