--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTService_GetDistToTarget_C
local M = UnLua.Class()
local Screen = require "Widgets.Screen"
function M:ReceiveTickAI(_OwnerController,ControlledPawn,DeltaSeconds)
    local OtherActor = UE.UBTFunctionLibrary.GetBlackboardValueAsObject(self, self.InTargetActorKey)
    if not OtherActor then
        --Screen.Print("ERROR: Target actor is nil, check your blackboard key.")
        return
    end
    local Distance = ControlledPawn:GetDistanceTo( OtherActor)
    UE.UBTFunctionLibrary.SetBlackboardValueAsFloat(self, self.OutDistToTargetKey, Distance)
    end

return M