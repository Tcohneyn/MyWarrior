// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Shared/Enemy_Death_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
void UEnemy_Death_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UAnimMontage* MontageToPlay = MontagesArrayRandom(MontagesArray);
    PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageTask"), MontageToPlay);

    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontage);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontage);

    // 激活事件任务
    PlayMontageTask->ReadyForActivation();

        // GameplayCue
    GetWarriorAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(DeathGameplayCueTag);
}

UAnimMontage* UEnemy_Death_Base::MontagesArrayRandom(TArray<UAnimMontage*> MontagesArrayToRandom)
{
    int32 RandomIndex = FMath::RandRange(0, MontagesArrayToRandom.Num() - 1);
    return MontagesArrayToRandom[RandomIndex];

}

void UEnemy_Death_Base::OnMontage() 
{
    // 安全结束任务
    if (PlayMontageTask) PlayMontageTask->EndTask();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}