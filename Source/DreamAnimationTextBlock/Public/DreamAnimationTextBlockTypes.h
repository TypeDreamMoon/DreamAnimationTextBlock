#pragma once

#include "CoreMinimal.h"
#include "DreamAnimationTextBlockTypes.generated.h"

USTRUCT(BlueprintType)
struct FDreamTextAnimationSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Scale = FVector2D(1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Translation = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Shear = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Angle = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Opacity = 1.f;
};
