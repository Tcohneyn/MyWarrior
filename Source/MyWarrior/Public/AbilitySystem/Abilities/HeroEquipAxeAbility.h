// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "HeroEquipAxeAbility.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
class AWarriorHeroWeapon;

UCLASS()
class MYWARRIOR_API UHeroEquipAxeAbility : public UWarriorHeroGameplayAbility
{
    GENERATED_BODY()
public:
    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> NewWeaponHandles;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Equip")
    TObjectPtr<UAnimMontage> MontageToPlay;

    UPROPERTY(EditDefaultsOnly, Category = "Equip")
    FGameplayTag EventTag;

    UPROPERTY(EditDefaultsOnly, Category = "Equip")
    FGameplayTag WeaponTag;

    UPROPERTY(EditDefaultsOnly, Category = "Equip")
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

    void HandleEquipWeapon(AWarriorHeroWeapon* InWeaponToEquip);

    UFUNCTION(BlueprintCallable, Category = "Sequence")
    void RunSequenceTasks();

private:
    AWarriorHeroWeapon* CurrentHeroWeapon;
};
