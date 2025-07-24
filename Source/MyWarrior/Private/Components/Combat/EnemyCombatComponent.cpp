// Tcohneyn All Rights Reserved

#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"
#include "WarriorDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.Add(HitActor);

    // TODO: Implement block check
    bool IsValidBlock = false;

    const bool IsPlayerBlocking = false;
    const bool bIsMyAttackUnblockable = false;

    if (IsPlayerBlocking && !bIsMyAttackUnblockable)
    {
        // TODO:check if the block is valid
    }
    FGameplayEventData EventData;
    EventData.Instigator = GetOwningPawn();
    EventData.Target = HitActor;
    if (IsValidBlock)
    {
        // TODO: handle successful block
    }
    else
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Shared_Event_MeleeHit, EventData);
    }
}
