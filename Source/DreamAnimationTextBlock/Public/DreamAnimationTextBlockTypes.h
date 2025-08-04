// Copyright (c) 2022 Dream Moon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamAnimationTextBlockTypes.generated.h"

/**
 * 文本动画设置结构体
 * 用于定义文本动画的各种变换属性，包括缩放、平移、剪切、内边距、旋转角度和透明度
 */
USTRUCT(BlueprintType)
struct FDreamTextAnimationSet
{
	GENERATED_BODY()

public:
	/** 文本缩放因子，X和Y方向的缩放比例，默认值为(1.0, 1.0)表示不缩放 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextAnimationSet")
	FVector2D Scale = FVector2D(1.f, 1.f);

	/** 文本平移偏移量，X和Y方向的位移，默认值为(0.0, 0.0)表示不平移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextAnimationSet")
	FVector2D Translation = FVector2D(0.f, 0.f);

	/** 文本剪切变形量，X和Y方向的剪切变形，默认值为(0.0, 0.0)表示无剪切 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextAnimationSet")
	FVector2D Shear = FVector2D(0.f, 0.f);

	/** 文本内边距，X和Y方向的填充距离，默认值为(0.0, 0.0)表示无内边距 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextAnimationSet")
	FVector2D Padding = FVector2D(0.f, 0.f);

	/** 文本旋转角度，以度为单位，默认值为0.0表示不旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextAnimationSet")
	float Angle = 0.f;

	/** 文本透明度，取值范围0.0-1.0，0.0表示完全透明，1.0表示完全不透明，默认值为1.0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamTextAnimationSet")
	float Opacity = 1.f;
};

/**
 * 文本动画播放类型枚举
 * 定义文本动画的播放方式，包括顺序播放、同时播放和随机播放三种模式
 */
UENUM(BlueprintType)
enum class EDreamTextAnimationPlayType : uint8
{
	/** 顺序播放 - 按照预设顺序依次播放各个动画效果 */
	Sequentially UMETA(DisplayName = "顺序"),

	/** 同时播放 - 所有动画效果同时开始播放 */
	Simultaneously UMETA(DisplayName = "同时"),

	/** 随机播放 - 随机选择动画效果进行播放 */
	Randomly UMETA(DisplayName = "随机"),
};
