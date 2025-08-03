// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "DreamWidgetTweener.h"
#include "DreamWidgetTweenManager.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType, Transient)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenTickHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDreamWidgetTweenTickHelperComponent();
	UPROPERTY() TWeakObjectPtr<class UDreamWidgetTweenManager> Target = nullptr;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
};

UCLASS(NotBlueprintable, NotBlueprintType, Transient, NotPlaceable)
class DREAMANIMATIONTEXTBLOCK_API ADreamTweenTickHelperActor : public AActor
{
	GENERATED_BODY()

public:
	ADreamTweenTickHelperActor();
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaSeconds)override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason)override;
	UPROPERTY() TWeakObjectPtr<class UDreamWidgetTweenManager> Target = nullptr;
private:
	void OnDreamWidgetTweenManagerCreated(class UDreamWidgetTweenManager* DreamWidgetTweenManager);
	FDelegateHandle OnDreamWidgetTweenManagerCreatedDelegateHandle;

	void SetupTick(UDreamWidgetTweenManager* DreamWidgetTweenManager);
};

// This class is only for spawn ADreamWidgetTweenTickHelperActor for game world
UCLASS(NotBlueprintable, NotBlueprintType, Transient)
class DREAMANIMATIONTEXTBLOCK_API UDreamTweenTickHelperWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void PostInitialize()override;
};

DECLARE_EVENT_OneParam(UDreamWidgetTweenManager, FDreamWidgetTweenManagerCreated, class UDreamWidgetTweenManager*);

UCLASS(NotBlueprintable, BlueprintType, Transient)
class DREAMANIMATIONTEXTBLOCK_API UDreamWidgetTweenManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:	

	//~USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End of USubsystem interface

	//~FTickableObjectBase interface
	void Tick(EDreamWidgetTweenTickType TickType, float DeltaTime);
	
	UFUNCTION(BlueprintPure, Category = DreamWidgetTween, meta = (WorldContext = "WorldContextObject", DisplayName = "Get DreamWidgetTween Instance"))
	static UDreamWidgetTweenManager* GetDreamWidgetTweenInstance(UObject* WorldContextObject);
	static FDreamWidgetTweenManagerCreated OnDreamWidgetTweenManagerCreated;
private:
	/** current active tweener collection*/
	UPROPERTY(VisibleAnywhere, Category=DreamWidgetTween)TArray<TObjectPtr<UDreamWidgetTweener>> tweenerList;
	void OnTick(EDreamWidgetTweenTickType TickType, float DeltaTime, float UnscaledDeltaTime);
	FDreamWidgetTweenUpdateMulticastDelegate updateEvent;
	bool bTickPaused = false;
public:
	UE_DEPRECATED(5.1, "Use Tweener->SetTickType(EDreamWidgetTweenTickType::Manual) then call this->ManualTick.")
	/** Use "CustomTick" instead of UE4's default Tick to control your tween animations. Call "DisableTick" function to disable UE4's default Tick function, then call this CustomTick function.*/
	UFUNCTION(BlueprintCallable, Category = DreamWidgetTween, meta=(DeprecatedFunction, DeprecationMessage = "Use Tweener->SetTickType(EDreamWidgetTweenTickType::Manual) then call this->ManualTick."))
	void CustomTick(float DeltaTime);
	UE_DEPRECATED(5.1, "Use Tweener->SetTickType(EDreamWidgetTweenTickType::Manual) then call this->ManualTick.")
	/**
	 * Disable default Tick function, so you can pause all tween or use CustomTick to do your own tick and use your own DeltaTime.
	 * This will only pause the tick with current DreamWidgetTweenManager instance, so after load a new level, default Tick will work again, and you need to call DisableTick again if you want to disable tick.
	 */ 
	UFUNCTION(BlueprintCallable, Category = DreamWidgetTween, meta = (DeprecatedFunction, DeprecationMessage = "Use Tweener->SetTickType(EDreamWidgetTweenTickType::Manual) then call this->ManualTick."))
	void DisableTick();
	UE_DEPRECATED(5.1, "Use Tweener->SetTickType(EDreamWidgetTweenTickType::Manual) then call this->ManualTick.")
	/**
	 * Enable default Tick if it is disabled.
	 */
	UFUNCTION(BlueprintCallable, Category = DreamWidgetTween, meta = (DeprecatedFunction, DeprecationMessage = "Use Tweener->SetTickType(EDreamWidgetTweenTickType::Manual) then call this->ManualTick."))
	void EnableTick();


	UFUNCTION(BlueprintCallable, Category = DreamWidgetTween)
	void ManualTick(float DeltaTime);

	/**
	 * Kill all tweens
	 */
	UFUNCTION(BlueprintCallable, Category = DreamWidgetTween)
	void KillAlDreamWidgetTweens(bool callComplete = false);

	/**
	 * Is the tweener is currently tweening? 
	 * @param item tweener item
	 */
	static bool IsTweening(UObject* WorldContextObject, UDreamWidgetTweener* item);
	/**
	 * Kill the tweener if it is tweening.
	 * @param item tweener item
	 * @param callComplete true-execute onComplete event.
	 */
	static void KillIfIsTweening(UObject* WorldContextObject, UDreamWidgetTweener* item, bool callComplete);
	/**
	 * Remove tweener from list, so the tweener will not be managed by this DreamWidgetTweenManager.
	 * @param item tweener item
	 */
	static void RemoveTweener(UObject* WorldContextObject, UDreamWidgetTweener* item);

	static UDreamWidgetTweener* To(UObject* WorldContextObject, const FDreamWidgetTweenFloatGetterFunction& getter, const FDreamWidgetTweenFloatSetterFunction& setter, float endValue, float duration);
	static UDreamWidgetTweener* To(UObject* WorldContextObject, const FDreamWidgetTweenDoubleGetterFunction& getter, const FDreamWidgetTweenDoubleSetterFunction& setter, double endValue, float duration);
	static UDreamWidgetTweener* To(UObject* WorldContextObject, const FDreamWidgetTweenVectorGetterFunction& getter, const FDreamWidgetTweenVectorSetterFunction& setter, const FVector& endValue, float duration);
	static UDreamWidgetTweener* To(UObject* WorldContextObject, const FDreamWidgetTweenVector2DGetterFunction& getter, const FDreamWidgetTweenVector2DSetterFunction& setter, const FVector2D& endValue, float duration);

	static UDreamWidgetTweener* VirtualTo(UObject* WorldContextObject, float duration);
	static UDreamWidgetTweener* DelayFrameCall(UObject* WorldContextObject, int delayFrame);
	static UDreamWidgetTweener* UpdateCall(UObject* WorldContextObject);

	static class UDreamWidgetTweenerSequence* CreateSequence(UObject* WorldContextObject);

	UE_DEPRECATED(5.2, "Use DreamWidgetTweenBPLibrary.UpdateCall instead.")
	static FDelegateHandle RegisterUpdateEvent(UObject* WorldContextObject, const FDreamWidgetTweenUpdateDelegate& update);
	UE_DEPRECATED(5.2, "Use DreamWidgetTweenBPLibrary.UpdateCall instead of RegisterUpdateEvent, and KillIfIsTweening for returned tweener instead of this UnregisterUpdateEvent.")
	static void UnregisterUpdateEvent(UObject* WorldContextObject, const FDelegateHandle& delegateHandle);
};
