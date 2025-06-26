// Tcohneyn All Rights Reserved

#include "AbilitySystem/Abilities/Shared/SharedSpawnWeapon.h"
#include "Components/Combat/PawnCombatComponent.h"


void USharedSpawnWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetAvatarActorFromActorInfo();  // 设置所有者
    SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector(0, 0, 0));

    if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(WeaponClassToSpawn, SpawnTransform, SpawnParams))
    {
        FAttachmentTransformRules NewRules(EAttachmentRule::SnapToTarget,  // Location
            EAttachmentRule::KeepRelative,                                 // Rotation
            EAttachmentRule::KeepWorld,                                    // Scale
            true                                                           // 自动更新子组件位置
        );
        NewActor->AttachToComponent(GetOwningComponentFromActorInfo(), NewRules, SocketNameToAttachTo);
        if (UPawnCombatComponent* CombatComp = GetPawnCombatComponentFromActorInfo())
        {
            CombatComp->RegisterSpawnedWeapon(WeaponTagToRegister, Cast<AWarriorWeaponBase>(NewActor), RegisterasEquippedWeapon);

            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        }
        else
        {
            NewActor->Destroy();
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        }
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
}
