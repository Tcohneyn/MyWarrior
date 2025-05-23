// Tcohneyn All Rights Reserved

#ifndef GAMEPLAYTAGS_H
// 防止头文件被多次包含，使用宏定义进行保护
#define GAMEPLAYTAGS_H

#include "NativeGameplayTags.h"
// 包含原生游戏标签的头文件，以便使用UE4中的游戏标签功能

namespace WarriorGameplayTags
{
// 定义一个命名空间WarriorGameplayTags，用于存放与战士游戏相关的标签

/** Input Tag **/
// 注释说明以下定义的是输入相关的标签

MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move) 
// 使用MYWARRIOR_API宏定义一个外部声明的游戏标签InputTag_Move，用于标识移动输入
// UE_DECLARE_GAMEPLAY_TAG_EXTERN宏用于声明一个可以在其他地方定义的游戏标签

MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look)
// 使用MYWARRIOR_API宏定义一个外部声明的游戏标签InputTag_Look，用于标识视角查看输入

}  // namespace WarriorGameplayTags
// 结束命名空间WarriorGameplayTags

#endif  // !GAMEPLAYTAGS_H
// 结束头文件保护宏定义
