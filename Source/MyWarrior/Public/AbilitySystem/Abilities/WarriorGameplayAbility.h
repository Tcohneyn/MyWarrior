// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorGameplayAbility.generated.h"

class UPawnCombatComponent;

UENUM(BlueprintType) 
enum class EWarriorAbilityActivationPolicy : uint8
{
    OnTriggered,
    OnGiven
};

UCLASS()
class MYWARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, Category = "WarriorAbility")
    EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

    //~ Begin UGameplayAbility Interface
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~ End UGameplayAbility Interface

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
