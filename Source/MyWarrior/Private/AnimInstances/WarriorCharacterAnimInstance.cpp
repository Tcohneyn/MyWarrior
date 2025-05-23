// Tcohneyn All Rights Reserved

#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// 初始化动画实例
void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
    // 尝试获取当前动画实例所属的角色，并将其转换为AWarriorBaseCharacter类型
    OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());
    // 如果成功获取到角色
    if (OwningCharacter)
    {
        // 获取角色的移动组件
        OwningMovementComponent = OwningCharacter->GetCharacterMovement();
    }
}

// 更新动画实例，线程安全
void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    // 如果没有角色或移动组件，直接返回
    if (!OwningCharacter || !OwningMovementComponent)
    {
        return;
    }

    // 获取角色的水平速度（二维速度大小）
    GroundSpeed = OwningCharacter->GetVelocity().Size2D();
    // 判断角色是否有加速度（加速度的平方大于0）
    bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}