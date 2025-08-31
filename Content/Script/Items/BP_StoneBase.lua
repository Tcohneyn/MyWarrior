--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_StoneBase_C
local M = UnLua.Class()

function M:BP_OnStoneConsumed()
   UE.UNiagaraFunctionLibrary.SpawnSystemAtLocation(self, self.ConsumeFX, self:K2_GetActorLocation(),UE.FRotator(0,0,0), UE.FVector(1,1,1), true, true, self.PoolingMethod, true)
   UE.UGameplayStatics.PlaySoundAtLocation(self, self.ConsumeSound, self:K2_GetActorLocation())
   self:K2_DestroyActor()
end

return M
