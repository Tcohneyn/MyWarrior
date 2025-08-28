// Tcohneyn All Rights Reserved

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "WarriorGameplayTags.h"
// 当某个技能输入被按下时触发
void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())// 输入标签无效则直接返回
    {
        return;
    }
    // 遍历所有可激活的技能
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        // 如果该技能没有绑定这个输入标签则跳过
        if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
        // 如果这个输入是“可切换类技能”（如开关技能），并且该技能已激活
        if (InputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
        {
            // 再次按下则取消技能
            CancelAbilityHandle(AbilitySpec.Handle);

        }
        else
        {
            // 否则尝试激活技能
            TryActivateAbility(AbilitySpec.Handle);
        }
    }
}
// 当某个技能输入被松开时触发
void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
    // 输入无效 或者 输入标签不是“必须按住才能维持”的类型，则直接返回
    if (!InInputTag.IsValid() || !InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
    {
        return;
    }
    // 遍历所有可激活的技能
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        // 找到绑定该输入标签并且正在激活的技能
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
        {
            // 松开按键时，取消技能
            CancelAbilityHandle(AbilitySpec.Handle);
        }
    }
}
// 给角色授予武器技能（包括普通和特殊技能）
void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities,
    const TArray<FWarriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities,int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
    if (InDefaultWeaponAbilities.IsEmpty())
    {
        return;
    }
    // 遍历默认武器技能
    for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
    {
        if (!AbilitySet.IsValid()) continue;
        // 构造一个技能实例
        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
        // 授予该技能，并保存句柄
        OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
    }
    // 遍历特殊武器技能
    for (const FWarriorHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
    {
        if(!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
    }
}
// 移除之前授予的武器技能
void UWarriorAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(
    UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
    if (InSpecHandlesToRemove.IsEmpty()) return;
    // 遍历所有需要移除的技能句柄
    for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
    {
        if (SpecHandle.IsValid())
        {
            ClearAbility(SpecHandle);
        }
    }
    InSpecHandlesToRemove.Empty();// 清空数组，避免重复释放
}
// 尝试通过标签激活某个技能
bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
    check(AbilityTagToActivate.IsValid());
    // 查找所有和该标签匹配的技能
    TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);
    // 如果找到了技能
    if (!FoundAbilitySpecs.IsEmpty())
    {
        // 随机选取一个技能（如果有多个绑定相同标签）
        const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
        FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];
        check(SpecToActivate);
        // 如果该技能当前没有激活，则尝试激活
        if (!SpecToActivate->IsActive())
        {
            return TryActivateAbility(SpecToActivate->Handle);
        }
    }
    return false;// 没找到或激活失败
}
