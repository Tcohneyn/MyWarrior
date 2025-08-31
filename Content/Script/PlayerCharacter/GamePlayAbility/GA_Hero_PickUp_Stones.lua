--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_PickUp_Stones_C
local M = UnLua.Class()

function M:K2_ActivateAbility()
   local ExecuteTask = UE.UAbilityTask_ExecuteTaskOnTick.ExecuteTaskOnTick(self)
   ExecuteTask.OnAbilityTaskTick:Add(self, M.OnTick)
   ExecuteTask:ReadyForActivation()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.EventTag)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
end

function M:OnTick(_DeltaTime)
   self:CollectStones()
end

function M:OnEventReceived(_Payload)
    self:ConsumeStones()
end
return M