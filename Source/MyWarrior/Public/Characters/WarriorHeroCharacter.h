// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#pragma once
// 确保头文件只被包含一次，防止重复定义

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "WarriorHeroCharacter.generated.h"
// Unreal Engine的宏，用于生成反射代码，便于编辑器识别和管理类

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
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
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ALLowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    UHeroCombatComponent* HeroCombatComponent;
#pragma endregion
    // 区域标记结束

#pragma region Inputs
    // 区域标记，用于分组相关代码，便于阅读和维护

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData", meta = (ALLowPrivateAccess = "true"))
    TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;
    // 定义一个指向输入配置数据资产的指针，用于存储输入映射配置

    void Input_Move(const FInputActionValue& InputActionValue);
    void Input_Look(const FInputActionValue& InputActionValue);

    void Input_AbilityInputPressesd(FGameplayTag InputTag);
    void Input_AbilityInputReleased(FGameplayTag InputTag);
#pragma endregion
    // 区域标记结束

public:
    FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
};
