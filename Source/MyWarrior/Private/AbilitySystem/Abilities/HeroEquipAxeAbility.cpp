// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/HeroEquipAxeAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorDebugHelper.h"

void UHeroEquipAxeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageTask"), MontageToPlay);

    WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);

    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontage);

    WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);

    // 激活事件任务
    PlayMontageTask->ReadyForActivation();
    WaitEventTask->ReadyForActivation();
}

void UHeroEquipAxeAbility::OnMontage()
{
    // 安全结束任务
    if (PlayMontageTask) PlayMontageTask->EndTask();
    if (WaitEventTask) WaitEventTask->EndTask();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHeroEquipAxeAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
    Debug::Print("OnGameplayEventReceived");
    UHeroCombatComponent* CombatComponent = GetHeroCombatComponentFromActorInfo();
    if (!CombatComponent) return;
    AWarriorWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCarriedWeaponByTag(WeaponTag);
    FAttachmentTransformRules NewRules(EAttachmentRule::SnapToTarget,  // Location
        EAttachmentRule::KeepRelative,                                 // Rotation
        EAttachmentRule::KeepWorld,                                    // Scale
              true                                                           // 自动更新子组件位置
    );
    CurrentWeapon->AttachToComponent(GetOwningComponentFromActorInfo(), NewRules, SocketName);
}
