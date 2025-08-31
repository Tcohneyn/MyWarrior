// Tcohneyn All Rights Reserved


#include "Items/PickUps/WarriorStoneBase.h"
#include "Characters/WarriorHeroCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

// 石头基类的消耗函数
// 当角色拾取并使用石头时，会将对应的 GameplayEffect 应用到角色自身
void AWarriorStoneBase::Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
    // 确保该石头有绑定的 GameplayEffect 类，否则在 Debug 下会触发断言崩溃
    check(StoneGameplayEffectClass);
    // 获取 GameplayEffect 类的默认对象 (Class Default Object, CDO)
    // GameplayEffect 用来定义一个效果（比如回血、加攻击力、护盾等）
    UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
    // 调用能力系统组件，将该效果应用到自己身上
    // 参数：
    // - EffectCDO：要应用的效果定义
    // - ApplyLevel：效果等级，决定效果的强度
    // - MakeEffectContext()：效果上下文（来源、目标、施放位置等信息）
    AbilitySystemComponent->ApplyGameplayEffectToSelf(
        EffectCDO,
        ApplyLevel,
        AbilitySystemComponent->MakeEffectContext()
    );
    // 通知蓝图：石头已被消耗
    // 蓝图里可以实现 BP_OnStoneConsumed，比如播放音效、UI提示、石头消失动画等
    BP_OnStoneConsumed();
}

void AWarriorStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AWarriorHeroCharacter* OverlappedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor))
    {
        OverlappedHeroCharacter->GetWarriorAbilitySystemComponent()->TryActivateAbilityByTag(WarriorGameplayTags::Player_Ability_PickUp_Stones);
    }
}