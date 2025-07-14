--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type ANS_ToggleWeaponCollision_C
local M = UnLua.Class()

function M:Received_NotifyBegin(MeshComp)
    local Owner = MeshComp:GetOwner()-- 获取骨骼网格组件的拥有者（通常为角色Actor）
    local CombatComponet = UE.UWarriorFunctionLibrary.BP_GetPawnCombatComponentFromActor(Owner)
    if CombatComponet then
    CombatComponet:ToggleWeaponCollision(true,self.ToggleDamageType)
else
    print("ERROR: Missing combat component on "..Owner:GetName())
end
    return false -- 返回 false 以阻止动画继续播放
end


function M:Received_NotifyEnd(MeshComp)
    local Owner = MeshComp:GetOwner()
    local CombatComponet = UE.UWarriorFunctionLibrary.BP_GetPawnCombatComponentFromActor(Owner)
    CombatComponet:ToggleWeaponCollision(false,self.ToggleDamageType)
    return false -- 返回 false 以阻止动画继续播放
end

return M