// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChanged, TSoftObjectPtr<UTexture2D>,SoftWeaponIcon);

UCLASS()
class MYWARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
    FOnPercentChangedDelegate OnCurrentRageChanged;

    UPROPERTY(BlueprintCallable, BlueprintAssignable)
    FOnEquippedWeaponChanged OnEquippedWeaponChanged;
};
