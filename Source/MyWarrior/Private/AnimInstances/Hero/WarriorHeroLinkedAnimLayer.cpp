// Tcohneyn All Rights Reserved

#include "AnimInstances/Hero/WarriorHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogHeroLinkedAnimLayer, All, All);

UWarriorHeroAnimInstance* UWarriorHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
    // 1. 检查组件有效性
    if (!GetOwningComponent() || !GetOwningComponent()->IsValidLowLevel())
    {
        UE_LOG(LogHeroLinkedAnimLayer, Warning, TEXT("OwningComponent is not valid"));
        return nullptr;
    }

    // 2. 检查动画实例有效性
    UAnimInstance* BaseInstance = GetOwningComponent()->GetAnimInstance();
    if (!BaseInstance || !BaseInstance->IsA<UWarriorHeroAnimInstance>())
    {
        UE_LOG(LogHeroLinkedAnimLayer, Warning, TEXT("BaseInstance is not a UWarriorHeroAnimInstance"));
        return nullptr;
    }

    // 3. 安全转换
    return Cast<UWarriorHeroAnimInstance>(BaseInstance);
}
