// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamAnimationTextBlockTypes.h"
#include "DreamWidgetTweener.h"
#include "Engine/DataAsset.h"
#include "DreamTextBlockAnimationSetting.generated.h"

/**
 * 
 */
UCLASS()
class DREAMANIMATIONTEXTBLOCK_API UDreamTextBlockAnimationSetting : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDreamTextAnimationSet Start = FDreamTextAnimationSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDreamTextAnimationSet End = FDreamTextAnimationSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDreamWidgetTweenEase Ease = EDreamWidgetTweenEase::OutCubic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "Ease==EDreamWidgetTweenEase::CurveFloat", EditConditionHides))
	UCurveFloat* CurveFloat = nullptr;
};
