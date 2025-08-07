--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_Projectile_Base_C
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    if not self.ProjectileSpawnSound:IsValid() then
        return 
    end
    local Location = self:K2_GetActorLocation()
    local GPstatic =UE.UGameplayStatics
    GPstatic.PlaySoundAtLocation(self, self.ProjectileSpawnSound, Location)
    GPstatic.SpawnSoundAttached(self.ProjectileFlySound,self:K2_GetRootComponent(),"None",UE.FVector(0,0,0),UE.FRotator(0,0,0),UE.EAttachLocation.KeepRelativeOffset,true)
    local SpawnRotation = UE.UKismetMathLibrary.Conv_VectorToRotator(self:GetActorForwardVector())
    UE.UNiagaraFunctionLibrary.SpawnSystemAtLocation(self,self.ProjectileSpawnFx, Location,SpawnRotation)
end
function M:BP_OnSpawnProjectileHitFX(HitLocation)
   if self.ProjectileImpactSound:IsValid() then
        -- 播放命中音效
        local World = self:GetWorld()
        if World then
            UE.UGameplayStatics.PlaySoundAtLocation(World, self.ProjectileImpactSound, HitLocation)
        end
            UE.UNiagaraFunctionLibrary.SpawnSystemAtLocation(self,self.ProjectileImpactFx, HitLocation)
    end
end
return M
