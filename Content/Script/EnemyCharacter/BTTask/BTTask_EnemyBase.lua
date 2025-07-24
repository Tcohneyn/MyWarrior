--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTTask_EnemyBase_C
local BTTask_EnemyBase_C = UnLua.Class()

function BTTask_EnemyBase_C:ReceiveExecuteAI(OwnerController, ControlledPawn)
    -- 把蓝图的 Event Receive Execute AI 对应到 Lua 的 ReceiveExecuteAI
    if not (self.OwningEnemyCharacter and self.OwningEnemyCharacter:IsValid()) then
        -- 蓝图中 IsValid 检查失败后直接 Finish Execute (失败)
        UE.UKismetSystemLibrary.PrintString(self, "Error: OwningEnemyCharacter is not valid.")
        local EnemyChar = ControlledPawn:Cast(UE.AWarriorEnemyCharacter)
        if not EnemyChar then
            -- cast 失败，Finish Execute (失败)
            UE.UKismetSystemLibrary.PrintString(self, "Error: ControlledPawn is not AWarriorEnemyCharacter.")
            self:FinishExecute(false)
            return 
        end
   -- 赋值给 OwningEnemyCharacter
    self.OwningEnemyCharacter = EnemyChar
    end
    -- 调用“真正”的任务执行逻辑（即蓝图的 On Enemy Execute Task）
    self:OnEnemyExecuteTask()

    -- 最后 Finish Execute 成功
    self:FinishExecute(true)
    --return
    --self.OwningEnemyCharacter = nil
end

function BTTask_EnemyBase_C:OnEnemyExecuteTask()
    -- 这里可以添加具体的任务执行逻辑
end

return BTTask_EnemyBase_C
