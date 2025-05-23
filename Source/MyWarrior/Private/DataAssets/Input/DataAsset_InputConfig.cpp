// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#include "DataAssets/Input/DataAsset_InputConfig.h"
// 包含头文件 "DataAssets/Input/DataAsset_InputConfig.h"，用于访问该头文件中定义的类和函数

// UDataAsset_InputConfig::FindNativeInputActionByTag 函数定义
// 该函数用于根据输入标签查找原生的输入动作
UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InputTag) const
{
    // 遍历 NativeInputActions 数组中的每一个 FWarriorInputActionConfig 结构体
    for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
    {
        // 检查当前遍历的 InputActionConfig 的 InputTag 是否与传入的 InputTag 相等
        // 并且 InputActionConfig 的 InputAction 是否不为空
        if (InputActionConfig.InputTag == InputTag && InputActionConfig.InputAction)
        {
            // 如果条件满足，返回当前 InputActionConfig 的 InputAction
            return InputActionConfig.InputAction;
        }
    }
    // 如果遍历完所有元素都没有找到匹配的输入动作，返回 nullptr
    return nullptr;
}