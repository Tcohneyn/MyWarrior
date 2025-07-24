--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GC_Enemy_Hit_Sound_Stick_C
local M = UnLua.Class()
function M:OnExecute(MyTarget,Parameters)
local location = MyTarget:K2_GetActorLocation()
    UE.UKismetSystemLibrary.PlaySoundAtLocation(self, self.Sound, location)
   return true
end
return M