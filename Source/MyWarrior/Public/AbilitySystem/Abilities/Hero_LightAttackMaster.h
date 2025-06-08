// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Hero_LightAttackMaster.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class MYWARRIOR_API UHero_LightAttackMaster : public UWarriorHeroGameplayAbility
{
    GENERATED_BODY()

public:
    void ResetAttackComboCount();

    UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
    FGameplayTag JumpTag;
protected:
    UFUNCTION()
    void OnMontage();

    UFUNCTION()
    void RunSequenceTasks();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = "Light Attack")
    int CurrentLightAttackComboCount = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Light Attack")
    TMap<int, UAnimMontage*> AttackMontagesMap;
private:
    UAbilityTask_PlayMontageAndWait* PlayMontageTask;
    float ComboCountResetTime = 0.3f;
    FTimerHandle ComboCountResetTimerHandle;
    FTimerDelegate ResetAttackComboCountDelegate;

};
