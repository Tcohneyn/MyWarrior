// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "HeroUnequipAxeAbility.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
class AWarriorHeroWeapon;

UCLASS()
class MYWARRIOR_API UHeroUnequipAxeAbility : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Unequip")
    TObjectPtr<UAnimMontage> MontageToPlay;

    UPROPERTY(EditDefaultsOnly, Category = "Unequip")
    FGameplayTag EventTag;

    UPROPERTY(EditDefaultsOnly, Category = "Unequip")
    FGameplayTag WeaponTag;

    UPROPERTY(EditDefaultsOnly, Category = "Unequip")
    FName SocketName;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY()
    UAbilityTask_PlayMontageAndWait* PlayMontageTask;

    UPROPERTY()
    UAbilityTask_WaitGameplayEvent* WaitEventTask;

    UFUNCTION()
    void OnMontage();

    UFUNCTION()
    void OnGameplayEventReceived(FGameplayEventData Payload);

    UPROPERTY()
    TObjectPtr<AWarriorHeroWeapon> InWeaponToEquip;

    UPROPERTY()
    FWarriorHeroWeaponData CacheHeroWeaponData;

    void HandleUnequipWeapon(AWarriorHeroWeapon* InWeaponToEquip);

    UFUNCTION(BlueprintCallable, Category = "Sequence")
    void RunSequenceTasks();

private:
    AWarriorHeroWeapon* CurrentHeroWeapon;
};
