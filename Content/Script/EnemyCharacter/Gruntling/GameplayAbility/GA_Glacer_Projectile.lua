--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Glacer_Projectile_C
local M = UnLua.Class()
function M:K2_ActivateAbility()
    local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        self.MontagetoPlay)
    PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
    PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.EventTag,nil,false,false)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
end

function M:OnMontage()
    -- Montage 播放结束后结束 Ability（对应蓝图连接 EndAbility）
    self:K2_EndAbility()
end

 function M:OnEventReceived(Payload)
    -- 获取要生成的 Actor 的 UClass
    local ProjectileClass = self.ProjectileClass -- 需在 Lua 或 Blueprint 中赋值
    local Enemy = self:GetEnemyCharacterFromActorInfo()
    local World = self:GetWorld()
     if not World then
        return
     end

    -- 定义单独的值
    local SpawnLocation = self:GetOwningComponentFromActorInfo():GetSocketLocation("ProjectileSpawnSocket")
    local SpawnRotation = UE.UKismetMathLibrary.Conv_VectorToRotator(Enemy:GetActorForwardVector())
    local SpawnScale    = UE.FVector(1.0, 1.0, 1.0)
    local Transform = UE.UKismetMathLibrary.MakeTransform(SpawnLocation, SpawnRotation, SpawnScale)
    local Damage = self:BP_MakeEnemyDamageEffectSpecHandle(self.EffectClass,self.ProjectileDamageScalableFloat)
        -- 调用 SpawnActor
    local NewActor = World:SpawnActor(
        ProjectileClass,
        Transform,
        UE.ESpawnActorCollisionHandlingMethod.AlwaysSpawn,
        Enemy,
        Enemy
    )
    if NewActor then
    -- 调用在 C++/Blueprint Actor 中实现的初始化接口
       NewActor:InitDamageSpec(Damage)
    end
 end
return M