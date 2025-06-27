// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "Enemy_Death_Base.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UNiagaraSystem;
UCLASS()
class MYWARRIOR_API UEnemy_Death_Base : public UWarriorEnemyGameplayAbility
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = "Death")
    TArray<UAnimMontage*> MontagesArray;

    UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
    FGameplayTag DeathGameplayCueTag;

    UPROPERTY(EditDefaultsOnly, Category = "Death")
    TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System;

protected:
    UFUNCTION()
    void OnMontage();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    UAnimMontage* MontagesArrayRandom(TArray<UAnimMontage*> MontagesArrayToRandom);

private:
    UAbilityTask_PlayMontageAndWait* PlayMontageTask;
};
