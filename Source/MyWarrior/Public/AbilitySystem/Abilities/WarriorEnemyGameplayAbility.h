// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorEnemyGameplayAbility.generated.h"

class AWarriorEnemyCharacter;
class UEnemyCombatComponent;


UCLASS()
class MYWARRIOR_API UWarriorEnemyGameplayAbility : public UWarriorGameplayAbility
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    AWarriorEnemyCharacter* GetEnemyCharacterFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    FGameplayEffectSpecHandle BP_MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,
        const FScalableFloat& InDamageScalableFloat);

    /** 异步加载资源，Lua 直接调用 */
    UFUNCTION(BlueprintCallable, Category="Lua|Asset",meta = (WorldContext = "WorldContextObject"))
    static void AsyncLoadAsset(const UObject* WorldContextObject, TSoftClassPtr<UObject> AssetClass);
private:
    TWeakObjectPtr<AWarriorEnemyCharacter> CachedWarriorEnemyCharacter;

};