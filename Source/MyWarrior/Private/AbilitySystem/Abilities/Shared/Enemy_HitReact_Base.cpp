// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Shared/Enemy_HitReact_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Characters/WarriorEnemyCharacter.h"
void UEnemy_HitReact_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

    auto Task1 = [this, EventDataCopy = *TriggerEventData]
    {
        if (FaceAttackter)
        {
            FVector StartVector = GetEnemyCharacterFromActorInfo()->GetActorLocation();
            // FGameplayEventData EventData = *TriggerEventData;
            FVector TargetVector = EventDataCopy.Instigator->GetActorLocation();
            FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(StartVector, TargetVector);
            GetEnemyCharacterFromActorInfo()->SetActorRotation(NewRotation, ETeleportType::None);
        }
    };

    auto Task2 = [this]
    {
        // 所有分支均立即设置材质参数为1（激活受击效果）
        GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.f);
        if (HasHitReactMontageToPlay)
        {
            UAnimMontage* MontageToPlay = ShuffleMontage();
            PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageTask"), MontageToPlay);
            PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontage);
            PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontage);
            PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontage);
            PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontage);
            // 激活事件任务
            PlayMontageTask->ReadyForActivation();
            //GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.f);
        }
        else
        {
            //FLatentActionInfo LatentInfo;
            //LatentInfo.CallbackTarget = this;
            //LatentInfo.ExecutionFunction = FName("EndAbility");
            //GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.f);
            //UKismetSystemLibrary::Delay(GetWorld(), 0.2f, LatentInfo);
            //  使用 AbilityTask 安全延迟
            DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.2f);
            DelayTask->OnFinish.AddDynamic(this, &ThisClass::OnDelayFinished);
            DelayTask->ReadyForActivation();
        }
    };

    auto Task3 = [this, Handle, &ActorInfo, ActivationInfo]
    {
        FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GameplayEffectClass, 1.f);
        ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpecHandle);
    };
        // 顺序执行确保线程安全
    Task1();
    Task2();
    Task3();
}

void UEnemy_HitReact_Base::OnDelayFinished()
{
    if (IsActive())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UEnemy_HitReact_Base::OnMontage()
{
    // 安全结束任务
    if (PlayMontageTask) PlayMontageTask->EndTask();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

UAnimMontage* UEnemy_HitReact_Base::ShuffleMontage()
{
    int32 RandomIndex = FMath::RandRange(0, MontagesToPlay.Num() - 1);
    return MontagesToPlay[RandomIndex];
}
void UEnemy_HitReact_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // 仅活跃状态下重置材质
    if (IsActive())
    {
        GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0.f);
    }
    // 清理延迟任务
    if (DelayTask)
    {
        DelayTask->EndTask();
        DelayTask = nullptr;
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}