--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_EnemyCharacter_Base_C
local M = UnLua.Class()



function M:ReceiveBeginPlay()
   self:SetupTimeline()
   self:OnMontage()
end

function M:SetupTimeline()
    -- 先确保 Timeline 存在
    if not UE.UKismetSystemLibrary.IsValid(self.EntryRestoreTimeline) then
        print("[ERROR] EntryRestoreTimeline is nil or invalid!")
        return
    end

    -- 设置播放速度（防止除零错误）
    local totalTime = self.TotalEntryRestoreTime or 1.0
    if totalTime <= 0 then
        totalTime = 1.0
    end
    self.EntryRestoreTimeline:SetPlayRate(1.0 / totalTime)
    -- 检查委托有效性
    self:BindReverseTimelineUpdate(self.EntryRestoreTimeline)
end

function M:OnMontage()
    -- 检查数组是否有效且不为空
    if not self.EntryMontageToPlay or self.EntryMontageToPlay:Num() == 0 then
        UE.UKismetSystemLibrary.PrintString(self, "[OnMontage] No montage to play.")
        return
    end

    local length = self.EntryMontageToPlay:Num()
    local choice = math.random(1, length)  -- UE数组是0索引
    local MontagetoPlays = self.EntryMontageToPlay[choice]

    if not MontagetoPlays then
        UE.UKismetSystemLibrary.PrintString(self, "[OnMontage] Selected montage is nil.")
        return
    end

    self:PlayAnimMontage(MontagetoPlays)
    UE.UKismetSystemLibrary.PrintString(self, "[OnMontage] Playing montage: " .. MontagetoPlays:GetName())
end
-- function M:OnReverseUpdate(value)
--     -- 更新材质参数
--     UE.UKismetSystemLibrary.PrintString(self, "OnReverseUpdate: " .. tostring(value))
--     self.Mesh:SetScalarParameterValueOnMaterials("DissolveAmount", value)
    
--     local weapon = self.EnemyCombatComponent:GetCharacterCurrentEquippedWeapon()
--     if weapon:IsValid() then
--         weapon.WeaponMesh:SetScalarParameterValueOnMaterials("DissolveAmount", value)
--     end
-- end

return M
