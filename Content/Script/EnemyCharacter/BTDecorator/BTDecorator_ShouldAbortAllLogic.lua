--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTDecorator_ShouldAbortAllLogic_C
local M = UnLua.Class()
function M:PerformConditionCheckAI(OwnerController, ControlledPawn)
    -- 从 Blackboard 获取 Cached Target Actor
    local Target = UE.UBTFunctionLibrary.GetBlackboardValueAsActor(self, self.InTargetActorKey)
    if Target and Target:IsValid() then
        self.CachedTargetActor = Target
    end

    local ActorToCheck = self.CachedTargetActor
    if not (ActorToCheck and ActorToCheck:IsValid()) then
        --UE.UKismetSystemLibrary.PrintString(self, "Error: ActorToCheck invalid")
        return false
    end

    -- Tag 检查
    local hasTag = UE.UWarriorFunctionLibrary.NativeDoesActorHaveTag(ActorToCheck, self.TagToCheck)
    if hasTag then
        self.IsTargetActorDead=true
    end
    -- 第二个 Tag 判断 & 写黑板
    local hasTag2 = UE.UWarriorFunctionLibrary.NativeDoesActorHaveTag(ControlledPawn, self.TagToCheck)
    if hasTag2 then
        self.IsOwningAIDead=true
    end

    -- 返回逻辑判断
    local ok = hasTag or hasTag2
    local Distance = UE.UBTFunctionLibrary.GetBlackboardValueAsFloat(self,self.InDistToTargetKey)
    local DistanceCheck = UE.UKismetMathLibrary.NearlyEqual_FloatFloat(Distance, 0.0, 0.000001)
    return ok or DistanceCheck
end
return M