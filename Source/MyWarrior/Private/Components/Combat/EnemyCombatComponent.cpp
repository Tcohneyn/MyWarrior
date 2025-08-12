// Tcohneyn All Rights Reserved

#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "WarriorGameplayTags.h"
#include "WarriorDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.Add(HitActor);

    // TODO: 执行格挡检查
    bool IsValidBlock = false;

    const bool IsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor,WarriorGameplayTags::Player_Status_Blocking);
    //const bool bIsMyAttackUnblockable = false;
    const bool bIsMyAttackUnblockable =
        UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), WarriorGameplayTags::Enemy_Status_Unbloackable);

    if (IsPlayerBlocking && !bIsMyAttackUnblockable)
    {  //TODO:检查格挡是否有效
        IsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
    }
    FGameplayEventData EventData;
    EventData.Instigator = GetOwningPawn();
    EventData.Target = HitActor;
    if (IsValidBlock)
    {   //处理格挡成功
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, WarriorGameplayTags::Player_Event_SuccessfulBlock, EventData);
    }
    else
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Shared_Event_MeleeHit, EventData);
    }
}

void UEnemyCombatComponent::ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType) 
{
    AWarriorEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AWarriorEnemyCharacter>();

    check(OwningEnemyCharacter);

    UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
    UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

    check(LeftHandCollisionBox && RightHandCollisionBox);

    switch (ToggleDamageType)
    {
        case EToggleDamageType::LeftHand:
            LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
            break;

        case EToggleDamageType::RightHand:
            RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
            break;

        default: break;
    }

    if (!bShouldEnable)
    {
        OverlappedActors.Empty();
    }
}
