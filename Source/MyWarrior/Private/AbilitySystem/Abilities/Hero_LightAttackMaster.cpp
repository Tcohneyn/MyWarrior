// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Hero_LightAttackMaster.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "WarriorFunctionLibrary.h"
#include "Async/ParallelFor.h"
#include "WarriorDebugHelper.h"
void UHero_LightAttackMaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    GetWorld()->GetTimerManager().ClearTimer(ComboCountResetTimerHandle);

    UsedComboCount = CurrentLightAttackComboCount;

    UAnimMontage* MontageToPlay = AttackMontagesMap.FindRef(CurrentLightAttackComboCount);
    PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageTask"), MontageToPlay);

    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontage);

        // 激活事件任务
    PlayMontageTask->ReadyForActivation();
    RunSequenceTasks();
}


void UHero_LightAttackMaster::OnMontage()
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
void UHero_LightAttackMaster::ResetAttackComboCount() 
{
    CurrentLightAttackComboCount = 1;
    AActor* InActor = CastChecked<AActor>(GetHeroCharacterFromActorInfo());
    UWarriorFunctionLibrary::RemoveGameplayTagFromActor(InActor, JumpTag);
}
void UHero_LightAttackMaster::RunSequenceTasks() 
{
    auto Task1 = [this] { 

          WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);

          WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);

          WaitEventTask->ReadyForActivation();
        };

    auto Task2 = [this] { 
        if (CurrentLightAttackComboCount == AttackMontagesMap.Num())
        {
            ResetAttackComboCountDelegate.ExecuteIfBound();
        }
        else
        {
            if (CurrentLightAttackComboCount == AttackMontagesMap.Num() - 1)
            {
                AActor* InActor = CastChecked<AActor>(GetHeroCharacterFromActorInfo());
                UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(InActor, JumpTag);
                CurrentLightAttackComboCount++;
            }
            else
            {
                CurrentLightAttackComboCount++;
            }
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

void UHero_LightAttackMaster::OnGameplayEventReceived(FGameplayEventData Payload) 
{
    //Debug::Print(
    //    TEXT("Hitting") + Payload.Target->GetName() + TEXT("with light attack Current Combo Count") + FString::FromInt(UsedComboCount),
    //    FColor::Green);
    float InWeaponBaseDamage = GetHeroCombatComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
    FGameplayEffectSpecHandle InSpecHandle = MakeHeroDamageEffectSpecHandle(
        EffectClass, InWeaponBaseDamage, InCurrentAttackTypeTag, UsedComboCount);
    NativeApplyEffectSpecHandleToTarget(static_cast<AActor*>(Payload.Target), InSpecHandle);
}
