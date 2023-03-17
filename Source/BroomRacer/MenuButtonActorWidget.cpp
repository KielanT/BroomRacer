// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuButtonActorWidget.h"
#include "MenuActorUserWidget.h"
#include "Components/TextBlock.h"

// UMenuButtonActorWidget::UMenuButtonActorWidget()
// {
// 	//SetDrawAtDesiredSize(true);
// 	//SetWidgetSpace(EWidgetSpace::Screen);
// 	//SetCollisionEnabled(ECollisionEnabled::NoCollision);
// }

void UMenuButtonActorWidget::InitWidget()
{
	Super::InitWidget();
	
	
    if(GetWidget())
    {
        if(GetWidget()->IsA(UMenuActorUserWidget::StaticClass()))
        {
	        WidgetInst = Cast<UMenuActorUserWidget>(GetWidget());
        	if(WidgetInst->ButtonText)
        	{
        		WidgetInst->ButtonText->SetText(FText::FromString(ButtonText));
        	}
        }
    }
	
}
