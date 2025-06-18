// Tcohneyn All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "WarriorDebugHelper.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor) 
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }
    OverlappedActors.AddUnique(HitActor);
    //Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("hit") + HitActor->GetActorNameOrLabel(), FColor::Green);
    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        WarriorGameplayTags::Shared_Event_MeleeHit,
        Data
    );

}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor) 
{
    //Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("s weapon pulled from") + InteractedActor->GetActorNameOrLabel(), FColor::Red);
}
