--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_LightAttack_Axe_C
local M = UnLua.Class()

function M:WhileRageActive()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.WaitEventTag,nil,false,false)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
end

function M:OnEventReceived(Payload)
    -- 获取要生成的 Actor 的 UClass
    local ProjectileClass = self.ProjectileClass -- 需在 Lua 或 Blueprint 中赋值
    local Hero = self:GetHeroCharacterFromActorInfo()
    local World = self:GetWorld()
     if not World then
        return
     end

    -- 定义单独的值
    local SpawnWeaponmesh = self:GetHeroCombatComponentFromActorInfo():GetHeroCurrentEquippedWeapon().WeaponMesh
    local SpawnLocation = SpawnWeaponmesh:GetSocketLocation("RageSlashSocket")
    local SpawnRotationX = SpawnWeaponmesh:GetSocketRotation("RageSlashSocket").Roll
    local SpawnRotationZ = UE.UKismetMathLibrary.MakeRotFromX(Hero:GetActorForwardVector()).Yaw
    local SpawnRotation = UE.FRotator(0.0,SpawnRotationZ,SpawnRotationX)
    local SpawnScale    = UE.FVector(1.0, 1.0, 1.0)
    local Transform = UE.UKismetMathLibrary.MakeTransform(SpawnLocation, SpawnRotation, SpawnScale)
    local level = self:GetAbilityLevel()
    local Damage = self:MakeHeroDamageEffectSpecHandle(self.HeroEffectClass,self:GetHeroCombatComponentFromActorInfo():GetHeroCurrentEquippedWeaponDamageAtLevel(level),self.CurrentAttackTypeTag,self.UsedComboCount)
        -- 调用 SpawnActor
    local NewActor = World:SpawnActor(
        ProjectileClass,
        Transform,
        UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn,
        Hero,
        Hero
    )
    if NewActor then
    -- 调用在 C++/Blueprint Actor 中实现的初始化接口
       NewActor:InitDamageSpec(Damage)
    end
 end
return M