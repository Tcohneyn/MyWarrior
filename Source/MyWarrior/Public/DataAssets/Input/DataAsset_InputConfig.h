// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#pragma once
// 确保头文件只被包含一次，防止重复定义

#include "CoreMinimal.h"
// 包含Unreal Engine的核心最小头文件，提供基础功能
#include "Engine/DataAsset.h"
// 包含Unreal Engine的数据资产头文件，用于创建数据资产类
#include "GameplayTagContainer.h"
// 包含游戏玩法标签容器的头文件，用于处理游戏中的标签系统
#include "DataAsset_InputConfig.generated.h"
// 包含生成的头文件，用于序列化和反射

class UInputAction;
// 声明UInputAction类，表示输入动作
class UInputMappingContext;
// 声明UInputMappingContext类，表示输入映射上下文

USTRUCT(BlueprintType)
// 定义一个结构体，并使其可以在蓝图编辑器中使用
struct FWarriorInputActionConfig
{
    GENERATED_BODY()
    // 用于生成结构体的反射和序列化代码

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
    FGameplayTag InputTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UInputAction> InputAction;

    bool IsValid() const
    {
        return InputTag.IsValid() && InputAction;
    }
};

UCLASS()
// 定义一个类，并使其可以在Unreal Engine中使用
class MYWARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
    GENERATED_BODY()
    // 用于生成类的反射和序列化代码

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    // 定义一个属性，只能在默认设置中编辑，蓝图只读
    TObjectPtr<UInputMappingContext> DefaultMappingContext;
    // 默认的输入映射上下文，指向UInputMappingContext对象

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
    TArray<FWarriorInputActionConfig> NativeInputActions;


    UInputAction* FindNativeInputActionByTag(const FGameplayTag& InputTag) const;
    // 声明一个函数，用于通过游戏玩法标签查找原生的输入动作，返回UInputAction指针
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
    TArray<FWarriorInputActionConfig> AbilityInputActions;


};