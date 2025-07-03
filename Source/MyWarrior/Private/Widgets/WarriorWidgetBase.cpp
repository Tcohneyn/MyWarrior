// Tcohneyn All Rights Reserved


#include "Widgets/WarriorWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/HeroUIComponent.h"
#include "WarriorDebugHelper.h"

void UWarriorWidgetBase::NativeOnInitialized() 
{
    Super::NativeOnInitialized();
    if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
    {
        if (UHeroUIComponent* HeroUIComponet = PawnUIInterface->GetHeroUIComponent())
        {

            BP_OnOwningHeroUIComponentInitialized(HeroUIComponet);
            //HeroUIComponet->OnInitialized.Broadcast(HeroUIComponet);
        }
    }
}

void UWarriorWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor) 
{
    if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
    {
        UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
        checkf(EnemyUIComponent,TEXT("Failed to extrac an EnemyUIComponet fron %s"),*OwningEnemyActor->GetActorNameOrLabel());
        BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
    }
}




