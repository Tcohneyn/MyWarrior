// Tcohneyn All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelper.h"
//构造函数，初始化属性值
UWarriorAttributeSet::UWarriorAttributeSet() 
{
    InitCurrentHealth(1.f);
    InitMaxHealth(1.f);
    InitCurrentRage(1.f);
    InitMaxRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
}
//核心回调函数:当任何 GameplayEffect 修改了某个属性 时，GAS 会调用这个函数，传入修改后的结果数据
void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (!CachedPawnUIInterface.IsValid())
    {
        //接口访问PawnUI组件
        CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
    }

    checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"),
        *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
    //获取 Pawn 的 UI 组件
    UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

    checkf(PawnUIComponent, TEXT("Couldn't extrac a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
//生命值变化处理
    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
    {
        const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);
        //UI生命值广播
        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
    }
//怒气值变化处理
    if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
    {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

        SetCurrentRage(NewCurrentRage);
        if (GetCurrentRage() == GetMaxRage())
        {
            UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_Full);
        }
        else if (GetCurrentRage() == 0.f)
        {
            UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_None);
        }
        else
        {
            UWarriorFunctionLibrary::RemoveGameplayTagFromActor(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_Full);
            UWarriorFunctionLibrary::RemoveGameplayTagFromActor(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_None);
        }
        if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
        {
            ////UI怒气值广播
            HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
        }
    }
//伤害处理
    if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
    {
        const float OldHealth = GetCurrentHealth();
        const float DamageDone = GetDamageTaken();

        const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);

        const FString DebugString =
            FString::Printf(TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"), OldHealth, DamageDone, NewCurrentHealth);

        Debug::Print(DebugString, FColor::Green);

       PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());


        if (GetCurrentHealth() == 0.f)
        {
            UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);

        }
    }
}
