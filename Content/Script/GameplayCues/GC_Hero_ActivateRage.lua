--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GC_Hero_ActivateRage_C
local M = UnLua.Class()

function M:WhileActive(MyTarget, Parameters)
    self:RunSequence(MyTarget, Parameters)
    return false
end
-- 顺序执行任务（模拟Sequence节点）
function M:RunSequence(MyTarget, Parameters)
    -- 启动协程任务链

        -- 任务1：立即执行
        self:Task1(MyTarget)
        -- 任务2：延迟后执行
        self:Task2(Parameters)
        -- 任务3：立即执行
        self:Task3(MyTarget)

end
function M:Task1(MyTarget)
    local location = MyTarget:K2_GetActorLocation()
    UE.UNiagaraFunctionLibrary.SpawnSystemAtLocation(self,self.SystemTemplate, location)
end

function M:Task2(Parameters)
    self.AttachedRageFX = UE.UNiagaraFunctionLibrary.SpawnSystemAttached(self.SystemTemplate1,
        Parameters.TargetAttachComponent)
end
function M:Task3(MyTarget)
    local location = MyTarget:K2_GetActorLocation()
    UE.UGameplayStatics.PlaySoundAtLocation(self, self.Sound, location)
end

function M:OnRemove(MyTarget, Parameters)
    self.AttachedRageFX:K2_DestroyComponent(self)
    return false
end
return M
