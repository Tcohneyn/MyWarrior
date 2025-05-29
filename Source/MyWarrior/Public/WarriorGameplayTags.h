// Tcohneyn All Rights Reserved

#ifndef GAMEPLAYTAGS_H
// 防止头文件被多次包含，使用宏定义进行保护
#define GAMEPLAYTAGS_H

#include "NativeGameplayTags.h"


namespace WarriorGameplayTags
{
// 定义一个命名空间WarriorGameplayTags，用于存放与战士游戏相关的标签

/** Input Tag **/

MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move); 
MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);

/** Player Tag **/
MYWARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);
}  // namespace WarriorGameplayTags


#endif  // !GAMEPLAYTAGS_H
// 结束头文件保护宏定义
