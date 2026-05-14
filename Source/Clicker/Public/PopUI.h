// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PopUI.generated.h"

/**
 * 
 */
UCLASS()
class CLICKER_API UPopUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	/** Text block to display the popup value, bound to the widget in Blueprints */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	class UTextBlock* ScrollText;

public:
	/** Sets the value of the popup text with a "+" prefix */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPopupValue(int64 Value);
};
