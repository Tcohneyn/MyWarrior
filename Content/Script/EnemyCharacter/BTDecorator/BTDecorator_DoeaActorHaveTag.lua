--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTDecorator_DoeaActorHaveTag_C
local M = UnLua.Class()
function M:PerformConditionCheckAI(OwnerController, ControlledPawn)
    -- 从 Blackboard 读取 Actor
    local Key = self.InActorKeyToCheck -- 你在蓝图中设定的 Blackboard Key 的名称
    local ActorToCheck = UE.UBTFunctionLibrary.GetBlackboardValueAsActor(self, Key)
    -- 判空和合法性检查
    if not (ActorToCheck and ActorToCheck:IsValid()) then
        UE.UKismetSystemLibrary.PrintString(self, "Error: ActorToCheck is not valid.")
    end

    -- 如果设置了 "Inverse Condition Check"，则逻辑取反
    local HasTag = UE.UWarriorFunctionLibrary.NativeDoesActorHaveTag(ActorToCheck, self.TagToCheck)
    if HasTag then
        -- 如果没有设置 "Inverse Condition Check"，则返回 true
        if self.InverseConditionCheck then
            --UE.UKismetSystemLibrary.PrintString(self, "false ")
            return false
        else
            -- 如果设置了 "Inverse Condition Check"，则返回 false
            --UE.UKismetSystemLibrary.PrintString(self, "true ")
            return true
        end
    else
        -- 如果没有设置 "Inverse Condition Check"，则返回 false
        if self.InverseConditionCheck then
            --UE.UKismetSystemLibrary.PrintString(self, "true ")
            return true
        else
            -- 如果设置了 "Inverse Condition Check"，则返回 true
            --UE.UKismetSystemLibrary.PrintString(self, "false ")
            return false
        end
    end
end

return M
