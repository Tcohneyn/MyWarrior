// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UWarriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);
    if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo && !Spec.IsActive())
        {
            ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

void UWarriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo)
        {
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
        }
    }
}

void UWarriorGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}



UPawnCombatComponent* UWarriorGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UWarriorAbilitySystemComponent* UWarriorGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo() const
{
    return Cast<UWarriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UWarriorGameplayAbility::NativeApplyEffectSpecHandleToTarget(
    AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!TargetASC  || !InSpecHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid TargetASC or InSpecHandle!"));
        return FActiveGameplayEffectHandle();
    }
    ensure(TargetASC && InSpecHandle.IsValid());
    return GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

}

FActiveGameplayEffectHandle UWarriorGameplayAbility::BP_ApplyEffectSpecHandleToTarget(
    AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType)
{
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);
    OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;
    return ActiveGameplayEffectHandle;
}
void UWarriorGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
  //如果没打中任何目标，直接返回，不做任何处理
    if (InHitResults.IsEmpty())
    {
        return;
    }
  //获取当前技能的施放者（Pawn）
    APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
  //遍历所有命中的结果，一个一个目标处理
    for (const FHitResult& Hit : InHitResults)
    {
  //拿到被命中的 Actor，并且只关心 Pawn 类型（角色单位）
        if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
        {
          //检查目标是否是 敌对单位（友方单位不会吃到效果）
            if (UWarriorFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
            {
               //真正应用 GameplayEffect
                FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn,InSpecHandle);
               //检查效果是否成功应用
                if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
                {	
                    FGameplayEventData Data;
                    Data.Instigator = OwningPawn;
                    Data.Target = HitPawn;
					//向被击中的 Pawn 发送一个 GameplayEvent
                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
                        HitPawn,
                        WarriorGameplayTags::Shared_Event_HitReact,
                        Data
                    );
                }
            }
        }
    }
}

TArray<FHitResult> UWarriorGameplayAbility::NativeBoxTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start,
    const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
    bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, bool bIgnoreSelf)
{
    TArray<FHitResult> BoxTraceHits;
    UKismetSystemLibrary::BoxTraceMultiForObjects(WorldContextObject, Start,End,HalfSize, Orientation, ObjectTypes, bTraceComplex,
    ActorsToIgnore, DrawDebugType, BoxTraceHits, bIgnoreSelf);
    return BoxTraceHits;
}