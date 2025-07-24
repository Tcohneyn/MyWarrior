--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTDecorator_ComputeSuccessChance_C
local M = UnLua.Class()
function M:PerformConditionCheckAI(_OwnerController, ControlledPawn)

    local Weight = UE.UKismetMathLibrary.RandomFloatInRange(self.SuccessChanceMin, self.SuccessChanceMax)

    local bWeight = UE.UKismetMathLibrary.RandomBoolWithWeight(Weight)
    return bWeight
end
return M