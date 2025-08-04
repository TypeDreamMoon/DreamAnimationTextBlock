// Copyright 2019-Present LexLiu. All Rights Reserved.
// Modify By Dream Moon 2025

#include "DreamWidgetTweenManager.h"
#include "Tweener/DreamWidgetTweenerFloat.h"
#include "Tweener/DreamWidgetTweenerDouble.h"
#include "Tweener/DreamWidgetTweenerVector.h"
#include "Tweener/DreamWidgetTweenerVector2D.h"

#include "Tweener/DreamWidgetTweenerFrame.h"
#include "Tweener/DreamWidgetTweenerVirtual.h"
#include "Tweener/DreamWidgetTweenerUpdate.h"

#include "DreamWidgetTweenerSequence.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Tweener/DreamWidgetTweenerVector2D.h"

UDreamWidgetTweenTickHelperComponent::UDreamWidgetTweenTickHelperComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}
void UDreamWidgetTweenTickHelperComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UDreamWidgetTweenTickHelperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Target.IsValid())
	{
		Target->Tick((EDreamWidgetTweenTickType)((uint8)PrimaryComponentTick.TickGroup), DeltaTime);
	}
}
void UDreamWidgetTweenTickHelperComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

ADreamTweenTickHelperActor::ADreamTweenTickHelperActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}
void ADreamTweenTickHelperActor::BeginPlay()
{
	Super::BeginPlay();
	if (auto DreamWidgetTweenManager = UDreamWidgetTweenManager::GetDreamWidgetTweenInstance(this))
	{
		SetupTick(DreamWidgetTweenManager);
	}
	else
	{
		//If GameInstance subsystem not created yet, then register a event to wait it create
		OnDreamWidgetTweenManagerCreatedDelegateHandle = UDreamWidgetTweenManager::OnDreamWidgetTweenManagerCreated.AddUObject(this, &ADreamTweenTickHelperActor::OnDreamWidgetTweenManagerCreated);
	}
}
void ADreamTweenTickHelperActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Target.IsValid())
	{
		Target->Tick(EDreamWidgetTweenTickType::DuringPhysics, DeltaSeconds);
	}
}
void ADreamTweenTickHelperActor::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (OnDreamWidgetTweenManagerCreatedDelegateHandle.IsValid())
	{
		UDreamWidgetTweenManager::OnDreamWidgetTweenManagerCreated.Remove(OnDreamWidgetTweenManagerCreatedDelegateHandle);
	}
}
void ADreamTweenTickHelperActor::OnDreamWidgetTweenManagerCreated(UDreamWidgetTweenManager* DreamWidgetTweenManager)
{
	SetupTick(DreamWidgetTweenManager);
}
void ADreamTweenTickHelperActor::SetupTick(UDreamWidgetTweenManager* DreamWidgetTweenManager)
{
	auto CreateComp = [DreamWidgetTweenManager, this](ETickingGroup TickingGroup, FName Name) {
		auto TickComp_DuringPhysics = NewObject<UDreamWidgetTweenTickHelperComponent>(this, Name);
		TickComp_DuringPhysics->SetTickGroup(TickingGroup);
		TickComp_DuringPhysics->RegisterComponent();
		TickComp_DuringPhysics->Target = DreamWidgetTweenManager;
		this->AddInstanceComponent(TickComp_DuringPhysics);
		};
	CreateComp(ETickingGroup::TG_PrePhysics, TEXT("PrePhysics"));
	CreateComp(ETickingGroup::TG_PostPhysics, TEXT("PostPhysics"));
	CreateComp(ETickingGroup::TG_PostUpdateWork, TEXT("PostUpdateWork"));
	this->Target = DreamWidgetTweenManager;
}



bool UDreamTweenTickHelperWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const 
{
	if (auto World = Outer->GetWorld())
	{
		if (World->IsGameWorld())
		{
			return true;
		}
	}
	return false;
}
void UDreamTweenTickHelperWorldSubsystem::PostInitialize()
{
	if (auto World = GetWorld())
	{
		if (World->IsGameWorld())
		{
			World->SpawnActor<ADreamTweenTickHelperActor>();
		}
	}
}



FDreamWidgetTweenManagerCreated UDreamWidgetTweenManager::OnDreamWidgetTweenManagerCreated;
//~USubsystem interface
void UDreamWidgetTweenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	const UGameInstance* LocalGameInstance = GetGameInstance();
	check(LocalGameInstance);
}

void UDreamWidgetTweenManager::Deinitialize()
{
	tweenerList.Empty();
}

bool UDreamWidgetTweenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}
//~End of USubsystem interface

void UDreamWidgetTweenManager::Tick(EDreamWidgetTweenTickType TickType, float DeltaTime)
{
	if (bTickPaused)return;
	if (TickType == EDreamWidgetTweenTickType::Manual)
	{
		OnTick(TickType, DeltaTime, DeltaTime);
	}
	else
	{
		if (auto World = GetWorld())
		{
			OnTick(TickType, World->DeltaTimeSeconds, World->DeltaRealTimeSeconds);
		}
		else
		{
			OnTick(TickType, DeltaTime, DeltaTime);
		}
	}
}

#include "Kismet/GameplayStatics.h"
UDreamWidgetTweenManager* UDreamWidgetTweenManager::GetDreamWidgetTweenInstance(UObject* WorldContextObject)
{
	if (auto GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
		return GameInstance->GetSubsystem<UDreamWidgetTweenManager>();
	else
		return nullptr;
}

void UDreamWidgetTweenManager::OnTick(EDreamWidgetTweenTickType TickType, float DeltaTime, float UnscaledDeltaTime)
{
	auto count = tweenerList.Num();
	for (int32 i = 0; i < count; i++)
	{
		auto tweener = tweenerList[i];
		if (!IsValid(tweener))
		{
			tweenerList.RemoveAt(i);
			i--;
			count--;
		}
		else
		{
			if (tweener->GetTickType() != TickType)continue;
			if (tweener->ToNext(DeltaTime, UnscaledDeltaTime) == false)
			{
				tweenerList.RemoveAt(i);
				tweener->ConditionalBeginDestroy();
				i--;
				count--;
			}
		}
	}
	if (TickType == EDreamWidgetTweenTickType::DuringPhysics)
	{
		if (updateEvent.IsBound())
			updateEvent.Broadcast(DeltaTime);
	}
}

void UDreamWidgetTweenManager::CustomTick(float DeltaTime)
{
	OnTick(EDreamWidgetTweenTickType::DuringPhysics, DeltaTime, DeltaTime);
}

void UDreamWidgetTweenManager::DisableTick()
{
	bTickPaused = true;
}
void UDreamWidgetTweenManager::EnableTick()
{
	bTickPaused = false;
}
void UDreamWidgetTweenManager::ManualTick(float DeltaTime)
{
	Tick(EDreamWidgetTweenTickType::Manual, DeltaTime);
}
void UDreamWidgetTweenManager::KillAlDreamWidgetTweens(bool callComplete)
{
	for (auto item : tweenerList)
	{
		if (IsValid(item))
		{
			item->Kill(callComplete);
		}
	}
	tweenerList.Reset();
}
bool UDreamWidgetTweenManager::IsTweening(UObject* WorldContextObject, UDreamWidgetTweener* item)
{
	if (!IsValid(item))return false;

	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return false;

	return Instance->tweenerList.Contains(item);
}
void UDreamWidgetTweenManager::KillIfIsTweening(UObject* WorldContextObject, UDreamWidgetTweener* item, bool callComplete)
{
	if (IsTweening(WorldContextObject, item))
	{
		item->Kill(callComplete);
	}
}
void UDreamWidgetTweenManager::RemoveTweener(UObject* WorldContextObject, UDreamWidgetTweener* item)
{
	if (!IsValid(item))return;

	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return;
	Instance->tweenerList.Remove(item);
}
//float
UDreamWidgetTweener* UDreamWidgetTweenManager::To(UObject* WorldContextObject, const FDreamWidgetTweenFloatGetterFunction& getter, const FDreamWidgetTweenFloatSetterFunction& setter, float endValue, float duration)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerFloat>(WorldContextObject);
	tweener->SetInitialValue(getter, setter, endValue, duration);
	Instance->tweenerList.Add(tweener);
	return tweener;
}
//float
UDreamWidgetTweener* UDreamWidgetTweenManager::To(UObject* WorldContextObject, const FDreamWidgetTweenDoubleGetterFunction& getter, const FDreamWidgetTweenDoubleSetterFunction& setter, double endValue, float duration)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerDouble>(WorldContextObject);
	tweener->SetInitialValue(getter, setter, endValue, duration);
	Instance->tweenerList.Add(tweener);
	return tweener;
}
//vector
UDreamWidgetTweener* UDreamWidgetTweenManager::To(UObject* WorldContextObject, const FDreamWidgetTweenVectorGetterFunction& getter, const FDreamWidgetTweenVectorSetterFunction& setter, const FVector& endValue, float duration)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerVector>(WorldContextObject);
	tweener->SetInitialValue(getter, setter, endValue, duration);
	Instance->tweenerList.Add(tweener);
	return tweener;
}
//vector2d
UDreamWidgetTweener* UDreamWidgetTweenManager::To(UObject* WorldContextObject, const FDreamWidgetTweenVector2DGetterFunction& getter, const FDreamWidgetTweenVector2DSetterFunction& setter, const FVector2D& endValue, float duration)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerVector2D>(WorldContextObject);
	tweener->SetInitialValue(getter, setter, endValue, duration);
	Instance->tweenerList.Add(tweener);
	return tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenManager::VirtualTo(UObject* WorldContextObject, float duration)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerVirtual>(WorldContextObject);
	tweener->SetInitialValue(duration);
	Instance->tweenerList.Add(tweener);
	return tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenManager::DelayFrameCall(UObject* WorldContextObject, int delayFrame)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerFrame>(WorldContextObject);
	tweener->SetInitialValue(delayFrame);
	Instance->tweenerList.Add(tweener);
	return tweener;
}

UDreamWidgetTweener* UDreamWidgetTweenManager::UpdateCall(UObject* WorldContextObject)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerUpdate>(WorldContextObject);
	Instance->tweenerList.Add(tweener);
	return tweener;
}

UDreamWidgetTweenerSequence* UDreamWidgetTweenManager::CreateSequence(UObject* WorldContextObject)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return nullptr;

	auto tweener = NewObject<UDreamWidgetTweenerSequence>(WorldContextObject);
	Instance->tweenerList.Add(tweener);
	return tweener;
}

FDelegateHandle UDreamWidgetTweenManager::RegisterUpdateEvent(UObject* WorldContextObject, const FDreamWidgetTweenUpdateDelegate& update)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return FDelegateHandle();

	return Instance->updateEvent.Add(update);
}
void UDreamWidgetTweenManager::UnregisterUpdateEvent(UObject* WorldContextObject, const FDelegateHandle& delegateHandle)
{
	auto Instance = GetDreamWidgetTweenInstance(WorldContextObject);
	if (!IsValid(Instance))return;

	Instance->updateEvent.Remove(delegateHandle);
}