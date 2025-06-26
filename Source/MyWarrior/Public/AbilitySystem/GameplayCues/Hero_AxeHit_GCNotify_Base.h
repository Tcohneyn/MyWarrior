// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "Hero_AxeHit_GCNotify_Base.generated.h"


class USoundBase;

UCLASS()
class MYWARRIOR_API UHero_AxeHit_GCNotify_Base : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* Sound;


    virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

    virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
};
