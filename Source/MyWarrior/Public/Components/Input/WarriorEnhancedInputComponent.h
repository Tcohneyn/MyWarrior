// Tcohneyn All Rights Reserved
// 版权声明

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorEnhancedInputComponent.generated.h"

/**
 * UWarriorEnhancedInputComponent类定义
 * 继承自UEnhancedInputComponent，用于自定义 warrior 的输入逻辑
 */
UCLASS()
class MYWARRIOR_API UWarriorEnhancedInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()
    // 生成必要的宏和代码，用于UE4反射系统

public:
    /**
     * 绑定原生输入动作
     * @param InInputConfig 输入配置数据资产
     * @param InInputTag 输入标签
     * @param TriggerEvent 触发事件类型
     * @param ContextObject 上下文对象
     * @param Func 回调函数
     */
    template <class UserObject, typename CallbackFunc>
    void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent,
        UserObject* ContextObject, CallbackFunc Func);

    template <class UserObject, typename CallbackFunc>
    void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc,CallbackFunc InputRelasedFunc);
};

/**
 * 绑定原生输入动作的实现
 * @param InInputConfig 输入配置数据资产
 * @param InInputTag 输入标签
 * @param TriggerEvent 触发事件类型
 * @param ContextObject 上下文对象
 * @param Func 回调函数
 */
template <class UserObject, typename CallbackFunc>
inline void UWarriorEnhancedInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
    const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
    checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));
    // 检查输入配置数据资产是否为空，如果为空则输出错误信息并停止执行

    if (!InInputConfig) return;
    // 如果输入配置数据资产为空，直接返回

    if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
    // 尝试根据输入标签查找对应的输入动作
    {
        BindAction(FoundAction, TriggerEvent, ContextObject, Func);
        // 如果找到对应的输入动作，则绑定该动作到指定的触发事件和回调函数
    }
}

template <class UserObject, typename CallbackFunc>
inline void UWarriorEnhancedInputComponent::BindAbilityInputAction(
    const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputRelasedFunc)
{
    checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));
    for (const FWarriorInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
    {
        if (!AbilityInputActionConfig.IsValid()) continue;
        BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc,
            AbilityInputActionConfig.InputTag);
        BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputRelasedFunc,
            AbilityInputActionConfig.InputTag);
    }
}
