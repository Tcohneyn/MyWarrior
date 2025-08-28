--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_Heavy_WeaponAbility_Axe_C
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
    self:K2_CommitAbility()
    local HeroUIComp = self:GetHeroCharacterfromActorInfo().HeroUIComponent
    local CooldownTimeRemaining = self:GetCooldownTimeRemaining()
    --UE.UKismetSystemLibrary.PrintString(self, "CooldownTimeRemaining:" .. CooldownTimeRemaining)
    HeroUIComp.OnAbilityCooldownBegin:Broadcast(self.CooldownBeginTag, CooldownTimeRemaining,CooldownTimeRemaining)
end

function M:OnMontage()
    -- Montage 播放结束后结束 Ability（对应蓝图连接 EndAbility）
    self:K2_EndAbility()
end

function M:OnEventReceived(Payload)
   self:HandleApplyDamage(Payload)
end

function M:HandleApplyDamage(InPayload)
   local Location = self:GetHeroCharacterfromActorInfo():K2_GetActorLocation()
   local ForwardVector = self:GetHeroCharacterfromActorInfo():GetActorForwardVector()
   local Rotator = UE.UKismetMathLibrary.Conv_VectorToRotator(ForwardVector)
   local End = Location + ForwardVector * 200
   local BoxTraceHits = self:NativeBoxTraceMultiForObjects(self, Location, End, self.HalfSize, Rotator, self.ObjectTypes, false, nil, self.DrawDebug, true)
   local DamageEffectSpecHandle = self:MakeHeroDamageEffectSpecHandle(self.EffectClass,self:GetHeroCombatComponentFromActorInfo():GetHeroCurrentEquippedWeaponDamageAtLevel(self:GetAbilityLevel()),self.CurrentAttackTypeTag,3)
   self:ApplyGameplayEffectSpecHandleToHitResults(DamageEffectSpecHandle,BoxTraceHits)
end
return M