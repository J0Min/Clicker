// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/PopUI.h"
#include "Components/TextBlock.h"

void UPopUI::SetPopupValue(int64 Value)
{
    if (ScrollText)
    {
        FString FormattedString = FString::Printf(TEXT("+%lld"), Value);
        ScrollText->SetText(FText::FromString(FormattedString));
    }
}
