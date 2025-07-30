--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GC_Hero_Magic_Shield_C
local M = UnLua.Class()

function M:WhileActive(MyTarget, Parameters)
    self:RunSequence(MyTarget, Parameters)
    return false
end

-- 顺序执行任务（模拟Sequence节点）
function M:RunSequence(MyTarget, Parameters)
    -- 创建协程任务链
    -- 延迟0.1秒后执行 Task1

    self:Task1(MyTarget)
    -- 延迟0.2秒后执行 Task2

    self:Task2(Parameters)
end

function M:Task1(MyTarget)
    local location = MyTarget:K2_GetActorLocation()
    UE.UGameplayStatics.PlaySoundAtLocation(self, self.Sound, location)
end

function M:Task2(Parameters)
    self.Magicsystem = UE.UNiagaraFunctionLibrary.SpawnSystemAttached(self.SystemTemplate,
        Parameters.TargetAttachComponent,
        "MagicShieldSocket")
end

function M:OnRemove(MyTarget, Parameters)
    self.Magicsystem:K2_DestroyComponent(self)
    return false
end

return M
