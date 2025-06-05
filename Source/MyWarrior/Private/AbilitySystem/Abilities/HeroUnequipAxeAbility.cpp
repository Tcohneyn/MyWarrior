// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/HeroUnequipAxeAbility.h"
#include "AbilitySystem/Abilities/HeroEquipAxeAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "WarriorDebugHelper.h"
#include "AnimInstances/Hero/WarriorHeroLinkedAnimLayer.h"
#include "Async/ParallelFor.h"
#include "EnhancedInputSubsystems.h"
#include "Controllers/WarriorHeroController.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
void UHeroUnequipAxeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageTask"), MontageToPlay);

    WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
    UHeroCombatComponent* CombatComponent = GetHeroCombatComponentFromActorInfo();
    if (!CombatComponent) return;
    CombatComponent->CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;

    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontage);

    WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);

    // 激活事件任务
    PlayMontageTask->ReadyForActivation();
    WaitEventTask->ReadyForActivation();
}

void UHeroUnequipAxeAbility::OnMontage()
{
    // 安全结束任务
    if (PlayMontageTask) PlayMontageTask->EndTask();
    if (WaitEventTask) WaitEventTask->EndTask();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHeroUnequipAxeAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
    Debug::Print("UnGameplayEventReceived");
    UHeroCombatComponent* CombatComponent = GetHeroCombatComponentFromActorInfo();
    if (!CombatComponent) return;
    AWarriorWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCarriedWeaponByTag(WeaponTag);
    FAttachmentTransformRules NewRules(EAttachmentRule::SnapToTarget,  // Location
        EAttachmentRule::KeepRelative,                                 // Rotation
        EAttachmentRule::KeepWorld,                                    // Scale
        true                                                           // 自动更新子组件位置
    );
    CurrentWeapon->AttachToComponent(GetOwningComponentFromActorInfo(), NewRules, SocketName);
    CurrentHeroWeapon = CombatComponent->GetHeroCarriedWeaponByTag(WeaponTag);
    HandleUnequipWeapon(CurrentHeroWeapon);
}

void UHeroUnequipAxeAbility::HandleUnequipWeapon(AWarriorHeroWeapon* NewWeapon)
{
    check(NewWeapon);
    if (!NewWeapon) return;
    CacheHeroWeaponData = NewWeapon->HeroWeaponData;
    RunSequenceTasks();
}

void UHeroUnequipAxeAbility::RunSequenceTasks()
{
    // 定义三个并行任务（模拟Sequence的三个输出引脚）
    auto Task1 = [this]
    {
        TSubclassOf<UWarriorHeroLinkedAnimLayer> AnimLayer = CacheHeroWeaponData.WeaponAnimLayerToLink;
        if (GetOwningComponentFromActorInfo())
        {
            GetOwningComponentFromActorInfo()->UnlinkAnimClassLayers(AnimLayer);
        }
    };
    auto Task2 = [this]
    {
        ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
        auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
        if (Subsystem)
        {
            Subsystem->RemoveMappingContext(CacheHeroWeaponData.WeaponInputMappingContext);
        }
        
    };
    auto Task3 = [this]
    {
        UWarriorAbilitySystemComponent* ASC = GetWarriorAbilitySystemComponentFromActorInfo();
        check(ASC);
        if (!ASC) return;
        TArray<FGameplayAbilitySpecHandle> NewWeaponHandles = CurrentHeroWeapon->GetGrantedAbilitySpecHandles();
        if (ASC)
        {
            ASC->RemovedGrantedHeroWeaponAbilities(NewWeaponHandles);
        }
    };

    // 使用ParallelFor并行执行
    ParallelFor(3,
        [&](int32 Index)
        {
            switch (Index)
            {
                case 0: Task1(); break;
                case 1: Task2(); break;
                case 2: Task3(); break;
            }
        });
}
