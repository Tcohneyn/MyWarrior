--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type AIC_Enemy_Base_C
local M = UnLua.Class()

function M:ReceivePossess(PossessedPawn)
    if self.BehaviorTreeToRun and self.BehaviorTreeToRun:IsValid() then
        -- 确保行为树有效并且可以运行
        self:RunBehaviorTree(self.BehaviorTreeToRun)
    else
        -- 如果行为树无效，打印错误信息
        UE.UKismetSystemLibrary.PrintString(self, "Error: Behavior Tree is not valid or not set.")
    end
end

return M
