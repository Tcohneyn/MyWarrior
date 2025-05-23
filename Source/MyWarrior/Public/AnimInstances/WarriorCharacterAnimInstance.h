// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#pragma once
// 确保头文件只被包含一次，防止重复定义

#include "CoreMinimal.h"
// 包含Unreal Engine的核心最小头文件，提供基础类型和宏定义
#include "AnimInstances/WarriorBaseAnimInstance.h"
// 包含基础战士动画实例的头文件，继承自该类
#include "WarriorCharacterAnimInstance.generated.h"
// 包含生成的反射代码头文件，用于Unreal Engine的反射系统

class AWarriorBaseCharacter;
// 声明AWarriorBaseCharacter类，表示战士基础角色
class UCharacterMovementComponent;
// 声明UCharacterMovementComponent类，表示角色移动组件

UCLASS()
// UCLASS宏定义，表示这是一个Unreal Engine的类
class MYWARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
// 定义UWarriorCharacterAnimInstance类，继承自UWarriorBaseAnimInstance
{
    GENERATED_BODY()
    // GENERATED_BODY宏，用于生成序列化和反射相关的代码

public:
    // 公共成员函数和变量

    virtual void NativeInitializeAnimation() override;
    // 虚函数，用于初始化动画，override表示重写基类函数

    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
    // 虚函数，用于线程安全地更新动画，override表示重写基类函数

protected:
    // 受保护成员变量

    UPROPERTY()
    // UPROPERTY宏，表示这是一个Unreal Engine的属性
    TObjectPtr<AWarriorBaseCharacter> OwningCharacter;
    // 指向拥有此动画实例的战士基础角色的智能指针

    UPROPERTY()
    TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;
    // 指向拥有此动画实例的角色移动组件的智能指针

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    // UPROPERTY宏，指定属性在编辑器中的可见性和分类
    float GroundSpeed;
    // 表示角色在地面的速度

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    bool bHasAcceleration;
    // 表示角色是否有加速度
};