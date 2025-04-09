// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DreamTextChar.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamTextChar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

public:
	UFUNCTION(BlueprintCallable)
	void SetChar(FString InChar);

	UFUNCTION(BlueprintCallable)
	void SetFont(FSlateFontInfo InFont);
public:
	UPROPERTY(BlueprintReadOnly)
	FString Char;
};
