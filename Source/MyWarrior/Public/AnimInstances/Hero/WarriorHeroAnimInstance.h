// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#pragma once
// 确保头文件只被包含一次，防止重复定义

#include "CoreMinimal.h"
// 包含Unreal Engine的核心最小头文件，提供基础功能
#include "AnimInstances/WarriorCharacterAnimInstance.h"
// 包含自定义的战士角色动画实例头文件
#include "WarriorHeroAnimInstance.generated.h"
// 包含生成的头文件，用于序列化和反射

class AWarriorHeroCharacter;
// 声明AWarriorHeroCharacter类，表示战士英雄角色

UCLASS()
// UCLASS宏定义一个Unreal Engine类
class MYWARRIOR_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
// 定义UWarriorHeroAnimInstance类，继承自UWarriorCharacterAnimInstance
{
    GENERATED_BODY()
    // GENERATED_BODY宏，用于自动生成序列化和反射相关的代码

public:
    // 公共成员函数和变量

    virtual void NativeInitializeAnimation() override;
    // 虚函数，用于初始化动画，override表示重写基类函数

    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
    // 虚函数，用于线程安全地更新动画，override表示重写基类函数

protected:
    // 受保护成员变量

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
    // UPROPERTY宏定义一个Unreal Engine属性
    // VisibleDefaultsOnly：在默认视图中可见
    // BlueprintReadOnly：允许在蓝图中被读取
    // Category：指定属性在编辑器中的分类
    TObjectPtr<AWarriorHeroCharacter> OwningHeroCharacter;
    // 指向拥有该动画实例的战士英雄角色的指针

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    bool bShouldEnterRelaxState;
    // 布尔变量，表示是否应进入放松状态

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    // EditDefaultsOnly：只在默认属性编辑器中可见和可编辑
    float EnterRelaxStateThreshold = 5.f;
    // 浮点变量，表示进入放松状态的阈值，默认值为5

    float IdleElpasedTime;
    // 浮点变量，用于记录空闲状态经过的时间
};