// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Hero_HeavyAttackMaster.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "WarriorFunctionLibrary.h"
#include "Async/ParallelFor.h"
void UHero_HeavyAttackMaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    GetWorld()->GetTimerManager().ClearTimer(ComboCountResetTimerHandle);

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
    ParallelFor(1,
        [&](int32 Index)
        {
            switch (Index)
            {
                case 0: Task1(); break;
            }
        });
}