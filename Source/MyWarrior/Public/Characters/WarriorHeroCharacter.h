// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#pragma once
// 确保头文件只被包含一次，防止重复定义

#include "CoreMinimal.h"
// 包含Unreal Engine的核心最小头文件，提供基本的引擎功能
#include "Characters/WarriorBaseCharacter.h"
// 包含基础战士角色类头文件，AWarriorHeroCharacter继承自AWarriorBaseCharacter
#include "WarriorHeroCharacter.generated.h"
// Unreal Engine的宏，用于生成反射代码，便于编辑器识别和管理类

class USpringArmComponent;
// 声明弹簧臂组件类，用于相机跟随
class UCameraComponent;
// 声明相机组件类，用于角色视角
class UDataAsset_InputConfig;
// 声明输入配置数据资产类，用于存储输入映射配置
struct FInputActionValue;
// 声明输入动作值结构体，用于存储输入动作的值
class UHeroCombatComponent;

UCLASS()
// Unreal Engine的宏，定义一个UClass，表示这是一个可以被实例化的类
class MYWARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
// 定义AWarriorHeroCharacter类，继承自AWarriorBaseCharacter，MYWARRIOR_API是模块的导出宏
{
    GENERATED_BODY()
    // Unreal Engine的宏，用于生成类的序列化和反射代码

public:
    // 公共成员函数和变量
    AWarriorHeroCharacter();
    // 构造函数，用于初始化对象

protected:
    // 受保护成员函数和变量，只能在类内部和派生类中访问
    //~ Begin APawn Interface
    // 标记以下函数是实现APawn接口的
    virtual void PossessedBy(AController* NewController) override;
    // 重写APawn接口的PossessedBy函数，当角色被控制器拥有时调用
    //~ End APawn Interface
    // 标记以上函数是实现APawn接口的结束

    virtual void BeginPlay() override;
    // 重写BeginPlay函数，当游戏开始或角色被实例化时调用

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // 重写SetupPlayerInputComponent函数，用于设置玩家输入组件

private:
    // 私有成员函数和变量，只能在类内部访问
#pragma region Components
    // 区域标记，用于分组相关代码，便于阅读和维护

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ALLowPrivateAccess = "true"))
    // Unreal
    // Engine的宏，定义一个属性，VisibleAnywhere表示在编辑器中可见，BRidge表示可以在蓝图编辑器中访问，Category用于分类，meta用于元数据
    TObjectPtr<USpringArmComponent> CameraBoom;
    // 定义一个指向弹簧臂组件的指针，用于相机跟随

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ALLowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;
    // 定义一个指向相机组件的指针，用于角色视角

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UHeroCombatComponent> HeroCombatComponent;
#pragma endregion
    // 区域标记结束

#pragma region Inputs
    // 区域标记，用于分组相关代码，便于阅读和维护

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData", meta = (ALLowPrivateAccess = "true"))
    // Unreal Engine的宏，定义一个属性，EditDefaultsOnly表示只能在默认编辑器中编辑，Category用于分类，meta用于元数据
    TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;
    // 定义一个指向输入配置数据资产的指针，用于存储输入映射配置

    void Input_Move(const FInputActionValue& InputActionValue);
    // 定义一个输入移动的函数，参数为输入动作值

    void Input_Look(const FInputActionValue& InputActionValue);
    // 定义一个输入视角的函数，参数为输入动作值
#pragma endregion
    // 区域标记结束

public:
    FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
};
