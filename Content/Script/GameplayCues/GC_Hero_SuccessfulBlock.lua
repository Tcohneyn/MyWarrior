--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GC_Hero_SuccessfulBlock_C
local M = UnLua.Class()
function M:OnExecute(MyTarget,Parameters)
    UE.UKismetSystemLibrary.PrintString(MyTarget, "Block")
    local location = MyTarget:K2_GetActorLocation()
    UE.UKismetSystemLibrary.PlaySoundAtLocation(self, self.Sound, location)
    local location1 = Parameters.TargetAttachComponent:GetSocketLocation("MagicShieldSocket")
    UE.UNiagaraFunctionLibrary.SpawnSystemAtLocation(self,self.SystemTemplate, location1)
   return true
end
return M