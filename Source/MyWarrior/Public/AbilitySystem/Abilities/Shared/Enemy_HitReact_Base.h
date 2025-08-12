// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "Enemy_HitReact_Base.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitDelay;

UCLASS()
class MYWARRIOR_API UEnemy_HitReact_Base : public UWarriorEnemyGameplayAbility
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "HitReact")
    bool HasHitReactMontageToPlay = true;

    UPROPERTY(EditDefaultsOnly, Category = "HitReact")
    bool FaceAttackter = true;

    UPROPERTY(EditDefaultsOnly, Category = "HitReact")
    TArray<UAnimMontage*> MontagesToPlay;

    UPROPERTY(EditDefaultsOnly, Category = "HitReact")
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

protected:
    UFUNCTION()
    void OnMontage();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
    UPROPERTY()
    UAbilityTask_WaitDelay* DelayTask;

    UAbilityTask_PlayMontageAndWait* PlayMontageTask;

    UAnimMontage* ShuffleMontage();

    UFUNCTION()
    void OnDelayFinished();
};
