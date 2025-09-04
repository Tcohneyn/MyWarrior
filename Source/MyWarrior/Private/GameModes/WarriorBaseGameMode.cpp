// Tcohneyn All Rights Reserved


#include "GameModes/WarriorBaseGameMode.h"

AWarriorBaseGameMode::AWarriorBaseGameMode()
{
    // 允许该Actor（GameMode）接受Tick事件
    PrimaryActorTick.bCanEverTick = true;
    // 设置Actor在创建后立即开始接收Tick事件
    PrimaryActorTick.bStartWithTickEnabled = true;
}