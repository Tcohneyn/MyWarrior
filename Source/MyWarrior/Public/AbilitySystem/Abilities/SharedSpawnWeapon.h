// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "SharedSpawnWeapon.generated.h"

class AWarriorWeaponBase;

UCLASS()
class MYWARRIOR_API USharedSpawnWeapon : public UWarriorGameplayAbility
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AWarriorWeaponBase> WeaponClassToSpawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName SocketNameToAttachTo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTag WeaponTagToRegister;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool RegisterasEquippedWeapon;

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
