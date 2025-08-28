// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorGameplayAbility.generated.h"

namespace EDrawDebugTrace
{
enum Type : int;
}

class UPawnCombatComponent;
class WarriorAbilitySystemComponent;

UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
    OnTriggered,
    OnGiven
};

UCLASS()
class MYWARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, Category = "WarriorAbility")
    EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;
    //~ Begin UGameplayAbility Interface
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~ End UGameplayAbility Interface

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
    FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle,EWarriorSuccessType& OutSuccessType);
    //把某个 GameplayEffect（比如伤害、减益 Buff）应用到一组命中的目标身上，并在成功时触发 HitReact 事件
    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
    void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,const TArray<FHitResult>& InHitResults);

    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
    TArray<FHitResult> NativeBoxTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, bool bIgnoreSelf);
};
