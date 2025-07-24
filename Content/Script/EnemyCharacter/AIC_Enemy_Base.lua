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
    local BlackboardComp = self.Blackboard
    if not BlackboardComp then
        UE.UKismetSystemLibrary.PrintString(self, "Error: Blackboard is nil.")
        return
    end
    local Character = PossessedPawn:Cast(UE.ACharacter)
    if not Character then
        UE.UKismetSystemLibrary.PrintString(self, "Error: PossessedPawn is not ACharacter.")
        return
    end
    local Movement = Character.CharacterMovement
    if not Movement then
        UE.UKismetSystemLibrary.PrintString(self, "Error: GetCharacterMovement returned nil.")
        return
    end
   local DefaultMaxWalkSpeed = Movement.MaxWalkSpeed
   local Keyname = UE.UKismetSystemLibrary.MakeLiteralName("DefaultMaxWalkSpeed")
    if BlackboardComp then
        BlackboardComp:SetValueAsFloat(Keyname, DefaultMaxWalkSpeed)
    end
end

return M
