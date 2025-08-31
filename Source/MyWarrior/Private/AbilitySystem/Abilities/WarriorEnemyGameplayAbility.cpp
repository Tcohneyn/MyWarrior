// Tcohneyn All Rights Reserved


#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "WarriorGameplayTags.h"
#include "Items/PickUps/WarriorStoneBase.h"

AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedWarriorEnemyCharacter.IsValid())
    {
        CachedWarriorEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }
    return CachedWarriorEnemyCharacter.IsValid() ? CachedWarriorEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWarriorEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UWarriorEnemyGameplayAbility::BP_MakeEnemyDamageEffectSpecHandle(
    TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
    FGameplayEffectSpecHandle EffectSpecHandle =
        GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
        );
    return EffectSpecHandle;
}

void UWarriorEnemyGameplayAbility::AsyncLoadAsset(const UObject* WorldContextObject, TSoftClassPtr<UObject> AssetClass)
{
    
    if (!AssetClass.IsValid() && AssetClass.ToSoftObjectPath().IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("AsyncLoadClass: AssetClass is invalid"));
        return;
    }
    
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    const FSoftObjectPath SoftPath = AssetClass.ToSoftObjectPath();

    Streamable.RequestAsyncLoad(SoftPath, FStreamableDelegate::CreateLambda([SoftPath]()
    {
        UClass* LoadedClass = Cast<UClass>(SoftPath.ResolveObject());
        auto StoneClassBase = Cast<AWarriorStoneBase>(LoadedClass);
        if (LoadedClass)
        {
            StoneClassBase->OnStoneAssetLoaded.ExecuteIfBound(LoadedClass);   // 把 UClass* 回调出去
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AsyncLoadClass: Failed to cast to UClass for %s"), *SoftPath.ToString());
            StoneClassBase->OnStoneAssetLoaded.ExecuteIfBound(nullptr);
        }
    }));
}