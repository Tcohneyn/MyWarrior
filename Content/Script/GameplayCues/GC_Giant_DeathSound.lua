--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GC_Giant_DeathSound_C
local M = UnLua.Class("GC_MyCue_StaticBP")
-- Override OnExecute callback
function M:OnExecute(MyTarget, Parameters)
    -- 判断是否有合法目标
    UE.UKismetSystemLibrary.PrintString(self, " MyTarget")
    if MyTarget and MyTarget:IsValid() then
        local Location = MyTarget:GetActorLocation()

        -- 播放音效
        UE.UGameplayStatics.PlaySoundAtLocation(self, self.CueSound, Location)

        -- 如果要展示粒子或其他效果，也可以在这里触发
    else
        UE.UKismetSystemLibrary.PrintString(self, "[Cue] OnExecute: Invalid MyTarget")
    end

    -- 返回 true 表示 Cue 已处理
    return true
end

-- 如果需要响应 OnActive 或 OnRemove，也可以 override
-- function M:OnActive(MyTarget, Parameters) ... end
-- function M:OnRemove(MyTarget, Parameters) ... end
return M