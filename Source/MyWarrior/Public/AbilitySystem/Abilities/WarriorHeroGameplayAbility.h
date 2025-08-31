// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class UHeroUIComponent;
class AWarriorHeroCharacter;
class AWarriorHeroController;
class UHeroCombatComponent;

USTRUCT(BlueprintType)
struct FAbilityRemainingCooldownByTag
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Cooldown")
    float TotalCooldownTime = 0.f;

    UPROPERTY(BlueprintReadOnly, Category="Cooldown")
    float RemainingCooldownTime = 0.f;

    UPROPERTY(BlueprintReadOnly, Category="Cooldown")
    bool bIsOnCooldown = false;
};

UCLASS()
class MYWARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    AWarriorHeroController* GetHeroControllerFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UHeroUIComponent* GetHeroUIComponentFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag,int32 InUsedComboCount);

    //查询当前 AbilitySystemComponent 上是否存在对应冷却效果，并返回冷却的剩余时间和总时长
    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
    FAbilityRemainingCooldownByTag GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag);
private:
    TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;
    TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;
};
