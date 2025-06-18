// Tcohneyn All Rights Reserved

#include "Components/Combat/PawnCombatComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorDebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(
    FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
    checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named named %s has already been added as carried weapon"),
        *InWeaponTagToRegister.ToString());
    check(InWeaponToRegister);

    CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

    InWeaponToRegister->OnWeaponHitTarget.BindUObject(this,&ThisClass::OnHitTargetActor);
    InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

    if (bRegisterAsEquippedWeapon)
    {
        CurrentEquippedWeaponTag = InWeaponTagToRegister;
    }
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
    if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
    {
        if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
        {
            return *FoundWeapon;
        }
    }

    return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
    if (!CurrentEquippedWeaponTag.IsValid())
    {
        return nullptr;
    }

    return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}


void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
    if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
    {
        AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
        check(WeaponToToggle);
        if (bShouldEnable)
        {
            WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            //Debug::Print(WeaponToToggle->GetName() + TEXT(" Collision Enabled"), FColor::Green);
        }
        else
        {
            WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            //Debug::Print(WeaponToToggle->GetName() + TEXT(" Collision Disabled"), FColor::Red);
            OverlappedActors.Empty();
        }
    }
    //TODO:Handle body collision boxes
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor) {}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor) {}

//FGameplayTag UPawnCombatComponent::GetCharacterCurrentEquippedWeaponTag() const
//{
//    if (!CurrentEquippedWeaponTag.IsValid())
//    {
//        Debug::Print(TEXT("No weapon equipped"));
//        return FGameplayTag::EmptyTag;
//    }
//    Debug::Print(CurrentEquippedWeaponTag.ToString());
//    return CurrentEquippedWeaponTag;
//}
