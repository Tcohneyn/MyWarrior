--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_TargetLock_C
local M = UnLua.Class()

function M:K2_ActivateAbility()
    local Task = UE.UAbilityTask_ExecuteTaskOnTick.ExecuteTaskOnTick(self)
    if(not Task) then
        print("[ERROR] Task is nil!")
        return
    end
    Task.OnAbilityTaskTick:Add(self,M.Run)
    Task:ReadyForActivation()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.EventTag,nil,false,false)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
 end


 function M:Run(DeltaTime)
    self:OnTargetLockTick(DeltaTime)
 end

 function M:OnEventReceived(Payload)
--[[     local returnvalue = UE.UBlueprintGameplayTagLibrary.GetDebugStringFromGameplayTag(Payload.EventTag)
    UE.UKismetSystemLibrary.PrintString(self, "Event Received: " .. returnvalue) ]]
    self:SwitchTarget(Payload.EventTag)
 end
return M