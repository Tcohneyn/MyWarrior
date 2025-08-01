--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_Hit_React_C
local M = UnLua.Class()
function M:K2_ActivateAbilityFromEvent(EventData)
    local InVictim = self:GetHeroCharacterFromActorInfo()
    local HitResult = UE.UWarriorFunctionLibrary.ComputeHitReactDirectionTag(EventData.Instigator, InVictim)
    if not HitResult then
        UE.UKismetSystemLibrary.PrintString(self, "HitResult is invalid!")
        return
    end
    local s = string.format("%.2f", HitResult.Angle)
    local tag = UE.UBlueprintGameplayTagLibrary.GetDebugStringFromGameplayTag(HitResult.Tag)
    --[[ UE.UKismetSystemLibrary.PrintString(self, "AngleDiff" .. s .. " " .. tag) ]]
    local MontagetoPlay = self.MontageMap:Find(HitResult.Tag)
    local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        MontagetoPlay)
    PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
    PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
    local Target = self:GetOwningComponentFromActorInfo()
    Target:SetScalarParameterValueOnMaterials("HitFxSwitch", 1.0)
end

function M:OnMontage()
    -- Montage 播放结束后结束 Ability（对应蓝图连接 EndAbility）
    self:K2_EndAbility()
end

function M:K2_OnEndAbility()
    local Target = self:GetOwningComponentFromActorInfo()
    Target:SetScalarParameterValueOnMaterials("HitFxSwitch", 0.0)
end

return M
