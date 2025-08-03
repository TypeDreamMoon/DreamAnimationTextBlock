// Copyright (c) 2022 Dream Moon. All Rights Reserved.


#include "Components/DreamTextBlock.h"

#include "DreamAnimationTextBlockTools.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Kismet/KismetStringLibrary.h"

namespace
{
	// 小 helper 用来追踪多个完成事件，达到预期 count 时触发 FinalCallback。
	struct FMultiCompletionTracker
	{
		TSharedRef<int32> CompletedCount;
		int32 TargetCount;
		TFunction<void()> FinalCallback;
		int32 TokenAtCreation;

		FMultiCompletionTracker(int32 InTargetCount, TFunction<void()> InFinalCallback, int32 InToken)
			: CompletedCount(MakeShared<int32>(0))
			  , TargetCount(InTargetCount)
			  , FinalCallback(MoveTemp(InFinalCallback))
			  , TokenAtCreation(InToken)
		{
		}

		void Notify(UObject* Owner, int32 CurrentToken)
		{
			// 只有 token 匹配（即不是过期的播放）才计数
			if (CurrentToken != TokenAtCreation)
				return;

			(*CompletedCount)++;
			if (*CompletedCount >= TargetCount)
			{
				if (FinalCallback)
				{
					FinalCallback();
				}
			}
		}
	};

	static void CollectCharsInLine(UDreamTextLine* Line, TArray<UDreamTextChar*>& OutChars)
	{
		if (!Line || !Line->WrapBox) return;

		// WrapBox 的孩子是 UDreamTextChar
		const TArray<UWidget*> Children = Line->WrapBox->GetAllChildren();
		for (UWidget* Child : Children)
		{
			if (UDreamTextChar* Char = Cast<UDreamTextChar>(Child))
			{
				OutChars.Add(Char);
			}
		}
	}
}

void UDreamTextBlock::SetText(FText InText)
{
	Internal_SetText(InText, false);
}

void UDreamTextBlock::PlayAnimWithDuration(float InDuration, EDreamTextAnimationPlayType InPlayType)
{
	// 版本 token：旧的异步回调遇到不匹配就早退
	++AnimPlayToken;
	const int32 ThisPlayToken = AnimPlayToken;

	// 清理老定时器 / 旧序列
	if (AnimTimerHandle.State.IsValid())
	{
		AnimTimerHandle.Stop();
	}
	UWorld* World = GetWorld();
	if (World)
	{
		for (auto& H : RandomAnimTimerHandles)
		{
			if (H.IsValid())
			{
				World->GetTimerManager().ClearTimer(H);
			}
		}
	}
	RandomAnimTimerHandles.Empty();

	if (bPreInitialize)
	{
		InitializeAnimation();
	}

	if (!World)
	{
		OnAnimCompleted.Broadcast();
		return;
	}

	const int32 TotalChars = TextChars.Num();
	if (TotalChars == 0)
	{
		OnAnimCompleted.Broadcast();
		return;
	}

	TWeakObjectPtr<UDreamTextBlock> WeakSelf(this);

	switch (InPlayType)
	{
	case EDreamTextAnimationPlayType::Sequentially:
		{
			// 按行收集
			TArray<TArray<UDreamTextChar*>> LinesCharGroups;
			LinesCharGroups.Reserve(TextLines.Num());
			for (int32 LineIndex = 0; LineIndex < TextLines.Num(); ++LineIndex)
			{
				TArray<UDreamTextChar*> LineChars;
				CollectCharsInLine(TextLines[LineIndex], LineChars);
				if (LineChars.Num() > 0)
				{
					LinesCharGroups.Add(MoveTemp(LineChars));
				}
			}

			int32 CountChars = 0;
			for (const auto& Group : LinesCharGroups)
			{
				CountChars += Group.Num();
			}
			if (CountChars == 0)
			{
				OnAnimCompleted.Broadcast();
				return;
			}

			const float PerCharInterval = InDuration / static_cast<float>(CountChars);

			// completion tracker
			TSharedRef<FMultiCompletionTracker> Tracker = MakeShared<FMultiCompletionTracker>(
				CountChars,
				[this, ThisPlayToken]()
				{
					if (ThisPlayToken != AnimPlayToken) return;
					OnAnimCompleted.Broadcast();
				},
				ThisPlayToken);

			float CumulativeTime = 0.f; // 第一行无行延迟

			for (int32 LineIndex = 0; LineIndex < LinesCharGroups.Num(); ++LineIndex)
			{
				const TArray<UDreamTextChar*>& LineChars = LinesCharGroups[LineIndex];
				if (LineIndex > 0)
				{
					CumulativeTime += LineAnimationDelay;
				}

				for (UDreamTextChar* Element : LineChars)
				{
					if (!IsValid(Element))
						continue;

					const float CharDelay = CumulativeTime;
					CumulativeTime += PerCharInterval;

					auto TriggerLambda = [WeakSelf, ThisPlayToken, Element, Tracker]() mutable
					{
						if (!WeakSelf.IsValid()) return;
						if (ThisPlayToken != WeakSelf->AnimPlayToken) return;
						if (!IsValid(Element)) return;

						// 保留原来 delegate 链式调用
						auto OriginalDelegate = Element->OnAnimationCompletedCpp;
						Element->OnAnimationCompletedCpp.BindLambda([WeakSelf, ThisPlayToken, Element, OriginalDelegate, Tracker]() mutable
						{
							if (!WeakSelf.IsValid()) return;
							if (ThisPlayToken != WeakSelf->AnimPlayToken) return;
							if (!IsValid(Element)) return;

							if (OriginalDelegate.IsBound())
							{
								OriginalDelegate.Execute();
							}
							WeakSelf->OnCharAnimPlay.Broadcast(Element);
							Tracker->Notify(WeakSelf.Get(), ThisPlayToken);
						});

						Element->PlayAnimationWithCustomDuration(WeakSelf->CharAnimationDuration);
					};

					if (CharDelay <= KINDA_SMALL_NUMBER)
					{
						TriggerLambda();
					}
					else
					{
						FTimerHandle LocalHandle;
						FTimerDelegate TimerDel;
						TimerDel.BindLambda(MoveTemp(TriggerLambda));
						World->GetTimerManager().SetTimer(LocalHandle, TimerDel, CharDelay, false);
						RandomAnimTimerHandles.Add(LocalHandle);
					}
				}
			}
			break;
		}
	case EDreamTextAnimationPlayType::Simultaneously:
		{
			// 所有字符一起播放
			TSharedRef<FMultiCompletionTracker> Tracker = MakeShared<FMultiCompletionTracker>(
				TotalChars,
				[this, ThisPlayToken]()
				{
					if (ThisPlayToken != AnimPlayToken) return;
					OnAnimCompleted.Broadcast();
				},
				ThisPlayToken);

			for (UDreamTextChar* Element : TextChars)
			{
				if (!IsValid(Element))
					continue;

				auto OriginalDelegate = Element->OnAnimationCompletedCpp;
				Element->OnAnimationCompletedCpp.BindLambda([WeakSelf, ThisPlayToken, Element, OriginalDelegate, Tracker]() mutable
				{
					if (!WeakSelf.IsValid()) return;
					if (ThisPlayToken != WeakSelf->AnimPlayToken) return;
					if (!IsValid(Element)) return;

					if (OriginalDelegate.IsBound())
					{
						OriginalDelegate.Execute();
					}
					WeakSelf->OnCharAnimPlay.Broadcast(Element);
					Tracker->Notify(WeakSelf.Get(), ThisPlayToken);
				});

				Element->PlayAnimationWithCustomDuration(WeakSelf->CharAnimationDuration);
			}
			break;
		}
	case EDreamTextAnimationPlayType::Randomly:
		{
			// 先按行分组
			TArray<TArray<UDreamTextChar*>> LinesCharGroups;
			LinesCharGroups.Reserve(TextLines.Num());
			for (int32 LineIndex = 0; LineIndex < TextLines.Num(); ++LineIndex)
			{
				TArray<UDreamTextChar*> LineChars;
				CollectCharsInLine(TextLines[LineIndex], LineChars);
				if (LineChars.Num() > 0)
				{
					LinesCharGroups.Add(MoveTemp(LineChars));
				}
			}

			// completion tracker
			TSharedRef<FMultiCompletionTracker> Tracker = MakeShared<FMultiCompletionTracker>(
				TotalChars,
				[this, ThisPlayToken]()
				{
					if (ThisPlayToken != AnimPlayToken) return;
					OnAnimCompleted.Broadcast();
				},
				ThisPlayToken);

			for (int32 LineIndex = 0; LineIndex < LinesCharGroups.Num(); ++LineIndex)
			{
				const TArray<UDreamTextChar*>& LineChars = LinesCharGroups[LineIndex];
				float LineOffset = (LineIndex > 0) ? LineAnimationDelay * LineIndex : 0.f;

				for (UDreamTextChar* RawElement : LineChars)
				{
					if (!IsValid(RawElement))
						continue;

					const float RandomInner = FMath::FRandRange(0.f, InDuration);
					const float TotalDelay = LineOffset + RandomInner;

					TWeakObjectPtr<UDreamTextChar> ElementPtr(RawElement);
					FTimerHandle LocalHandle;
					FTimerDelegate TimerDel;

					TimerDel.BindLambda([WeakSelf, ThisPlayToken, ElementPtr, Tracker]() mutable
					{
						if (!WeakSelf.IsValid()) return;
						if (ThisPlayToken != WeakSelf->AnimPlayToken) return;

						UDreamTextChar* Element = ElementPtr.Get();
						if (!IsValid(Element)) return;

						auto OriginalDelegate = Element->OnAnimationCompletedCpp;
						Element->OnAnimationCompletedCpp.BindLambda([WeakSelf, ThisPlayToken, ElementPtr, OriginalDelegate, Tracker]() mutable
						{
							if (!WeakSelf.IsValid()) return;
							if (ThisPlayToken != WeakSelf->AnimPlayToken) return;

							UDreamTextChar* Elem = ElementPtr.Get();
							if (!IsValid(Elem)) return;

							if (OriginalDelegate.IsBound())
							{
								OriginalDelegate.Execute();
							}
							WeakSelf->OnCharAnimPlay.Broadcast(Elem);
							Tracker->Notify(WeakSelf.Get(), ThisPlayToken);
						});

						Element->PlayAnimationWithCustomDuration(WeakSelf->CharAnimationDuration);
					});

					World->GetTimerManager().SetTimer(LocalHandle, TimerDel, TotalDelay, false);
					RandomAnimTimerHandles.Add(LocalHandle);
				}
			}
			break;
		}
	default:
		break;
	}
}


void UDreamTextBlock::StopAnim(bool bReset, bool bEndState, bool bCallCompleted)
{
	// 取消序列定时器
	if (AnimTimerHandle.State.IsValid())
	{
		AnimTimerHandle.Stop();
	}

	// 取消随机定时器
	if (GWorld)
	{
		for (auto& Handle : RandomAnimTimerHandles)
		{
			if (Handle.IsValid())
			{
				GWorld->GetTimerManager().ClearTimer(Handle);
			}
		}
	}
	RandomAnimTimerHandles.Empty();

	for (auto Element : TextChars)
	{
		Element->StopAnim(bReset, bEndState, bCallCompleted);
	}
}

void UDreamTextBlock::PlayAnim(EDreamTextAnimationPlayType InPlayType)
{
	if (TextChars.Num() == 0)
	{
		OnAnimCompleted.Broadcast();
		return;
	}

	PlayAnimWithDuration(AnimationDuration, InPlayType);
}

bool UDreamTextBlock::IsAnimPlaying() const
{
	for (auto Element : TextChars)
	{
		if (Element->IsAnimPlaying())
			return true;
	}

	return false;
}

void UDreamTextBlock::InitializeAnimation()
{
	for (auto Element : TextChars)
	{
		Element->InitializeAnimation(AnimationSetting);
	}
}

void UDreamTextBlock::OnLastCharAnimationCompleted(const FDreamTextBlockSimpleDelegate& Delegate)
{
	TextChars.Last()->OnAnimationCompletedCpp.BindLambda([Delegate]()
	{
		Delegate.ExecuteIfBound();
	});
}

void UDreamTextBlock::NativePreConstruct()
{
	Super::NativePreConstruct();

	Internal_SetText(Text, true);
}

void UDreamTextBlock::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAutoPlay)
	{
		PlayAnim(PlayType);
	}
}

void UDreamTextBlock::ClearProperty()
{
	TextChars.Empty();
	TextLines.Empty();
	VerticalBox->ClearChildren();
	CurrentTextLine = nullptr;
}

UDreamTextLine* UDreamTextBlock::CreateNewLine()
{
	// Create LineWidget
	UDreamTextLine* LineWidget = CreateWidget<UDreamTextLine>(this, LineWidgetClass.LoadSynchronous());
	TextLines.Add(LineWidget);

	if (!LineWidget || !VerticalBox || !LineWidget->WrapBox) return nullptr;

	// Set LineWidget Properties
	LineWidget->SetFont(Font);
	LineWidget->SetAnimationSetting(AnimationSetting);
	LineWidget->WrapBox->SetExplicitWrapSize(bExplicitWrapSize);
	LineWidget->WrapBox->SetWrapSize(WrapSize);

	// Add LineWidget to VerticalBox
	UVerticalBoxSlot* VBSlot = VerticalBox->AddChildToVerticalBox(LineWidget);
	VBSlot->SetVerticalAlignment(LineVerticalAlignment);
	VBSlot->SetHorizontalAlignment(LineHorizontalAlignment);
	VBSlot->SetSize(LineSize);

	return LineWidget;
}

void UDreamTextBlock::Internal_SetText(FText InText, bool bIsPreConstruct)
{
	Text = InText;
	FString StrText = Text.ToString();

	ClearProperty();

	auto Push = [this, bIsPreConstruct](const FString& InChar)
	{
		UDreamTextChar* CharWidget = CurrentTextLine->PushChar(InChar, !bIsPreConstruct);
		CharWidget->SetTextPadding(TextPadding);
		return CharWidget;
	};


	for (auto Element : UKismetStringLibrary::GetCharacterArrayFromString(StrText))
	{
		if (Element == "\n")
		{
			CurrentTextLine = CreateNewLine();
		}
		else
		{
			if (CurrentTextLine)
			{
				if (UDreamTextChar* Char = Push(Element))
				{
					TextChars.Add(Char);
				}
			}
			else
			{
				CurrentTextLine = CreateNewLine();
				if (UDreamTextChar* Char = Push(Element))
					TextChars.Add(Char);
			}
		}
	}
}
