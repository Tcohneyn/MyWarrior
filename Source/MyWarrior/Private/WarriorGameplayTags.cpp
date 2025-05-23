// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#include "WarriorGameplayTags.h"
// 包含头文件 "WarriorGameplayTags.h"，该头文件可能定义了与战士游戏玩法标签相关的内容

namespace WarriorGameplayTags
{
// 定义一个命名空间 WarriorGameplayTags，用于封装与战士游戏玩法标签相关的代码，避免命名冲突

/** Input Tag **/
// 注释，表示以下定义的是输入相关的标签

UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")
// 使用宏 UE_DEFINE_GAMEPLAY_TAG 定义一个游戏玩法标签 InputTag_Move，其标签名称为 "InputTag.Move"
// 这个标签可能用于标识与角色移动相关的输入操作

UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look")
// 使用宏 UE_DEFINE_GAMEPLAY_TAG 定义一个游戏玩法标签 InputTag_Look，其标签名称为 "InputTag.Look"
// 这个标签可能用于标识与角色视角查看相关的输入操作
}  // namespace WarriorGameplayTags
// 结束命名空间 WarriorGameplayTags
