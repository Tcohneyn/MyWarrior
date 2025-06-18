// Tcohneyn All Rights Reserved
// 版权声明，表示代码的版权归属

#include "WarriorGameplayTags.h"


namespace WarriorGameplayTags
{
// 定义一个命名空间 WarriorGameplayTags，用于封装与战士游戏玩法标签相关的代码，避免命名冲突

/** Input Tag **/
UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe, "InputTag.EquipAxe");
UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe, "InputTag.UnequipAxe");
UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Axe, "InputTag.LightAttack.Axe");
UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Axe, "InputTag.HeavyAttack.Axe");

/** Player Tag **/
UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, "Player.Ability.Equip.Axe");
UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, "Player.Ability.Unequip.Axe");

UE_DEFINE_GAMEPLAY_TAG((Player_Ability_Attack_Light_Axe), "Player.Ability.Attack.Light.Axe");
UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, "Player.Ability.Attack.Heavy.Axe");

UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "Player.Weapon.Axe");
UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "Player.Event.Equip.Axe");
UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "Player.Event.Unequip.Axe");
UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "Player.Status.JumpToFinisher");

/** Enemy Tag **/
UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

/** Shared Tag **/
UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
}  // namespace WarriorGameplayTags
// 结束命名空间 WarriorGameplayTags
