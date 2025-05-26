// Tcohneyn All Rights Reserved

#include "Characters/WarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"

// 构造函数，设置默认值
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
    // 设置Actor是否可以Tick，这里设置为不可Tick
    PrimaryActorTick.bCanEverTick = false;
    // 设置Actor是否在开始时启用Tick，这里设置为不启用
    PrimaryActorTick.bStartWithTickEnabled = false;

    // 设置角色的Mesh不接收贴花
    GetMesh()->bReceivesDecals = false;

    // 创建并初始化WarriorAbilitySystemComponent子对象
    WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));

    // 创建并初始化WarriorAttributeSet子对象
    WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
}

// 获取能力系统组件
UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
    // 返回WarriorAbilitySystemComponent
    return GetWarriorAbilitySystemComponent();
}

// 当角色被控制时调用
void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
    // 调用基类的PossessedBy函数
    Super::PossessedBy(NewController);
    // 如果WarriorAbilitySystemComponent存在
    if (WarriorAbilitySystemComponent)
    {
        // 初始化能力系统组件的信息，将当前角色和控制器传递进去
        WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
        ensureMsgf(!CharacterStartUpData.IsNull(),TEXT("Forget to assign start up data to %s"), *GetName());
    }
}