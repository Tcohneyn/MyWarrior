// Tcohneyn All Rights Reserved

// 包含英雄动画实例的头文件
#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
// 包含英雄角色的头文件
#include "Characters/WarriorHeroCharacter.h"

// 初始化动画实例的函数
void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
    // 调用基类的初始化动画函数
    Super::NativeInitializeAnimation();
    // 检查当前拥有该动画实例的角色是否存在
    if (OwningCharacter)
    {
        // 将拥有该动画实例的角色转换为AWarriorHeroCharacter类型
        OwningCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
    }
}

// 更新动画的线程安全函数
void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    // 调用基类的线程安全更新动画函数
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
    // 检查是否有加速度
    if (bHasAcceleration)
    {
        // 如果有加速度，重置空闲时间
        IdleElpasedTime = 0.f;
        // 设置不进入放松状态
        bShouldEnterRelaxState = false;
    }
    else
    {
        // 如果没有加速度，累加空闲时间
        IdleElpasedTime += DeltaSeconds;
        // 判断是否应该进入放松状态
        bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
    }
}