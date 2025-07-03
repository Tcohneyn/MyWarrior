// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"

class UHeroUIComponent;

UCLASS()
class MYWARRIOR_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	

	protected:
    virtual void NativeOnInitialized();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Hero UI Component Initialized"))
    void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* OwningHeroUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UI Component Initialized"))
    void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);
	public:
    UFUNCTION(BlueprintCallable)
        void InitEnemyCreatedWidget(AActor* OwningEnemyActor);

};
