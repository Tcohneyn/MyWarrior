--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Warrior_Enemy_MeleeAttack_Base_C
local M = UnLua.Class()
function M:K2_ActivateAbility()
    local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        self.MontagetoPlay)
    PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
    PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
    self:RunSequenceTasks()
end

function M:OnMontage()
    self:K2_EndAbility()
end

function M:RunSequenceTasks()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.EventTag)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
end

function M:OnEventReceived(Payload)
--[[     local instigatorName = "InvalidInstigator"
    local targetName = "InvalidTarget"

    if Payload.Instigator and Payload.Instigator:IsValid() then
        instigatorName = Payload.Instigator:GetName()
    end

    if Payload.Target and Payload.Target:IsValid() then
        targetName = Payload.Target:GetName()
    end

    -- 4. 安全拼接字符串
    local msg = string.format("OnEventReceived: %s -> %s", instigatorName, targetName)
    UE.UKismetSystemLibrary.PrintString(self, msg) ]]
    if not Payload then
        return
    end
    self:HandleApplyDamage(Payload)
end

function M:HandleApplyDamage(InPayload)
    if not InPayload.Target or not InPayload.Target:IsValid() then
        UE.UKismetSystemLibrary.PrintString(self, "Target invalid!")
        return
    end
    local GESpecHandle = self:BP_MakeEnemyDamageEffectSpecHandle(self.EffectClass, self.InDamageScalableFloat)
    if not GESpecHandle then
        UE.UKismetSystemLibrary.PrintString(self, "EffectSpec invalid!")
        return
    end
    self:BP_ApplyEffectSpecHandleToTarget(InPayload.Target, GESpecHandle)
    local Context = UE.FGameplayEffectContextHandle()
    self:K2_ExecuteGameplayCue(self.WeaponHitSoundGameplayCueTag,Context)
end

return M
