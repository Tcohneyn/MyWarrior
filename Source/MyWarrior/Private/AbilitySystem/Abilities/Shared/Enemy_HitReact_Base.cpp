// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Shared/Enemy_HitReact_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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
            //FGameplayEventData EventData = *TriggerEventData;
            FVector TargetVector = EventDataCopy.Instigator->GetActorLocation();
            FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(StartVector, TargetVector);
            GetEnemyCharacterFromActorInfo()->SetActorRotation(NewRotation, ETeleportType::None);
        }
    };

    auto Task2 = [this]
    {
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
            GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.f);
        }
        else
        {
            FLatentActionInfo LatentInfo;
            LatentInfo.CallbackTarget = this;
            LatentInfo.ExecutionFunction = FName("EndAbility");
            GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.f);
            UKismetSystemLibrary::Delay(GetWorld(), 0.2f, LatentInfo);
        }
    };
    // 顺序执行确保线程安全
    Task1();
    Task2();
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
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0.f);
}