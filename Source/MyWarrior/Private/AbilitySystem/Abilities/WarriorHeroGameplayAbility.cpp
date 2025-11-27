// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "WarriorGameplayTags.h"
AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedWarriorHeroCharacter.IsValid())
    {
        CachedWarriorHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (!CachedWarriorHeroController.IsValid())
    {
        CachedWarriorHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
    }

    return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

UHeroUIComponent* UWarriorHeroGameplayAbility::GetHeroUIComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroUIComponent();
}
// 函数功能：创建英雄伤害效果的运行时规格（Spec）句柄
FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(
    TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
     // 1. 参数校验：确保传入的EffectClass有效（避免空指针）
    check(EffectClass);
    // 2. 创建效果上下文（Context）：存储伤害来源、触发者等关键信息
    FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);// 设置触发此效果的Ability实例[1](@ref)
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());// 设置伤害来源对象（如武器持有者）[1,7](@ref)
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());// 设置施加者（通常与来源相同）[1](@ref)
    // 3. 生成效果规格（Spec）：GameplayEffect的运行时实例
    FGameplayEffectSpecHandle EffectSpecHandle =
        GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);
    // 4. 动态配置伤害参数
    // 4.1 设置基础伤害值（通过SetByCaller机制动态传递）
    EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);
    // 4.2 设置攻击类型相关参数（如连击伤害加成）
    if (InCurrentAttackTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);// 连击数（可设计为连击越高伤害越高）
    }
    return EffectSpecHandle;
}

FAbilityRemainingCooldownByTag UWarriorHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag)
{
    FAbilityRemainingCooldownByTag Result;
    // 确保传入的标签有效，否则直接崩 assert
    check(InCooldownTag.IsValid());

    // 构造一个 GameplayEffectQuery，用来筛选带有这个 CooldownTag 的效果
    FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
    // 调用 AbilitySystemComponent 提供的 API：
    // 返回所有符合 Query 的 Active GameplayEffect，它们的 剩余时间 和 总时长
    TArray< TPair <float,float> > TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);
    // 如果找到了至少一个符合的效果
    if (!TimeRemainingAndDuration.IsEmpty())
    {
        Result.RemainingCooldownTime = TimeRemainingAndDuration[0].Key;// 取第一个效果的剩余时间
        Result.TotalCooldownTime= TimeRemainingAndDuration[0].Value;// 取第一个效果的总时长
    }
    // 返回值：如果剩余冷却时间大于 0，就说明当前在冷却中
    Result.bIsOnCooldown = Result.RemainingCooldownTime  > 0.f;
    return Result;
}
