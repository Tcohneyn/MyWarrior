// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Hero_HeavyAttackMaster.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "WarriorFunctionLibrary.h"
#include "Async/ParallelFor.h"
#include "WarriorDebugHelper.h"
void UHero_HeavyAttackMaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    GetWorld()->GetTimerManager().ClearTimer(ComboCountResetTimerHandle);

    UsedComboCount = CurrentHeavyAttackComboCount;

    AActor* InActor = CastChecked<AActor>(GetHeroCharacterFromActorInfo());
    if (UWarriorFunctionLibrary::NativeDoesActorHaveTag(InActor, JumpTag))
    {
        CurrentHeavyAttackComboCount = AttackMontagesMap.Num();
    }

    UAnimMontage* MontageToPlay = AttackMontagesMap.FindRef(CurrentHeavyAttackComboCount);
    PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageTask"), MontageToPlay);

    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontage);

    // 激活事件任务
    PlayMontageTask->ReadyForActivation();
    RunSequenceTasks();
}

void UHero_HeavyAttackMaster::OnMontage()
{
    // 安全结束任务
    if (PlayMontageTask) PlayMontageTask->EndTask();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    if (!ResetAttackComboCountDelegate.IsBound())
    {
        ResetAttackComboCountDelegate.Unbind();
    }
    ResetAttackComboCountDelegate.BindUObject(this, &ThisClass::ResetAttackComboCount);  // 绑定参数
    GetWorld()->GetTimerManager().SetTimer(ComboCountResetTimerHandle, ResetAttackComboCountDelegate, ComboCountResetTime, false);
}
void UHero_HeavyAttackMaster::ResetAttackComboCount()
{
    CurrentHeavyAttackComboCount = 1;
    AActor* InActor = CastChecked<AActor>(GetHeroCharacterFromActorInfo());
    UWarriorFunctionLibrary::RemoveGameplayTagFromActor(InActor, JumpTag);
}
void UHero_HeavyAttackMaster::RunSequenceTasks()
{
    auto Task1 = [this]
    {
        WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);

        WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);

        WaitEventTask->ReadyForActivation();
    };

    auto Task2 = [this]
    {
        if (CurrentHeavyAttackComboCount == AttackMontagesMap.Num())
        {
            ResetAttackComboCountDelegate.ExecuteIfBound();
        }
        else
        {
            CurrentHeavyAttackComboCount++;
        }
    };
    // 使用ParallelFor并行执行
    ParallelFor(2,
        [&](int32 Index)
        {
            switch (Index)
            {
                case 0: Task1(); break;
                case 1: Task2(); break;
            }
        });
}

void UHero_HeavyAttackMaster::OnGameplayEventReceived(FGameplayEventData Payload) 
{
    //Debug::Print(
    //    TEXT("Hitting") + Payload.Target->GetName() + TEXT("with heavy attack Current Combo Count") + FString::FromInt(UsedComboCount),
    //    FColor::Green);
    float InWeaponBaseDamage = GetHeroCombatComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
    FGameplayEffectSpecHandle InSpecHandle =
        MakeHeroDamageEffectSpecHandle(EffectClass, InWeaponBaseDamage, InCurrentAttackTypeTag, UsedComboCount);
    NativeApplyEffectSpecHandleToTarget(static_cast<AActor*>(Payload.Target), InSpecHandle);
}
