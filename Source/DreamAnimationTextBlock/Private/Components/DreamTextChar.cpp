// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DreamTextChar.h"

#include "Components/TextBlock.h"

void UDreamTextChar::SetChar(FString InChar)
{
	Char = InChar;
	TextBlock->SetText(FText::FromString(Char));
	
}

void UDreamTextChar::SetFont(FSlateFontInfo InFont)
{
	TextBlock->SetFont(InFont);
}
