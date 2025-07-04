// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#pragma once
// 确保头文件只被包含一次，防止重复定义

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "WarriorBaseCharacter.generated.h"
// 包含生成的头文件，用于序列化和反射

class UWarriorAbilitySystemComponent;
// 声明UWarriorAbilitySystemComponent类，用于前向声明
class UWarriorAttributeSet;
// 声明UWarriorAttributeSet类，用于前向声明
class UDataAsset_StartUpDataBase;


UCLASS()
class MYWARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface,public IPawnCombatInterface,public IPawnUIInterface
// 定义AWarriorBaseCharacter类，继承自ACharacter和IAbilitySystemInterface
{
    GENERATED_BODY()
    // GENERATED_BODY宏，用于生成序列化和反射所需的代码

public:
    // 公共接口部分

    // 构造函数，设置角色的默认属性
    AWarriorBaseCharacter();

    //~ Begin IAbilitySystemInterface Interface

    // 获取能力系统组件的虚函数重写
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    //~ End IAbilitySystemInterface Interface

    //~ Begin IPawnCombatComponent Interface
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
    //~ End IPawnCombatComponent Interface

    //~ Begin IPawnUIInterface Interface
    virtual UPawnUIComponent* GetPawnUIComponent() const override;
    //~ End  IPawnUIInterface Interface
protected:
    // 受保护成员变量和函数部分

    //~ Begin APawn Interface
    // 开始APawn接口的实现部分

    // 当角色被控制器拥有时的虚函数重写
    virtual void PossessedBy(AController* NewController) override;

    //~ End APawn Interface
    // 结束APawn接口的实现部分

    // 能力系统组件，可见于编辑器，可蓝图读取，分类为"AbilitySystem"
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    TObjectPtr<UWarriorAbilitySystemComponent> WarriorAbilitySystemComponent;

    // 角色属性集，可见于编辑器，可蓝图读取，分类为"AbilitySystem"
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    TObjectPtr<UWarriorAttributeSet> WarriorAttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
    TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
    // 公共接口部分

    // 获取战士能力系统组件的强制内联函数
    FORCEINLINE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }

    // 获取战士属性集的强制内联函数
    FORCEINLINE UWarriorAttributeSet* GetWarriorAttributeSet() const { return WarriorAttributeSet; }
};