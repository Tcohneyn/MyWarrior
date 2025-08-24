--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_Rage_C
local M = UnLua.Class()

function M:K2_ActivateAbility()
   local Character = self:GetHeroCharacterFromActorInfo()
   UE.UWarriorFunctionLibrary.AddGameplayTagToActorIfNone(Character,self.TagtoAdd)
   local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        self.MontagetoPlay)
    PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
    PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
    self:RunSequence()
end

function M:RunSequence(MyTarget, Parameters)
  --GameplayCue特效
    self:Task1()
  --拥有RageNone标签时取消能力
    self:Task2()
  --
    self:Task3()
end

function M:OnMontage()
    local Character = self:GetHeroCharacterFromActorInfo()
    UE.UWarriorFunctionLibrary.RemoveGameplayTagFromActor(Character,self.TagtoAdd)
end

function M:Task1()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.EventTag)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
end

function M:Task2()
    local WaitTask = UE.UAbilityTask_WaitGameplayTagAdded.WaitGameplayTagAdd(self, self.TagtoRemove)
    WaitTask.Added:Add(self, M.OnAdded)
    WaitTask:ReadyForActivation()
end

function M:Task3()
    self.ApplyRageHandle = self:BP_ApplyGameplayEffectToOwner(self.RageEffect, self:GetAbilityLevel(),1)
end
function M:OnEventReceived(Payload)
    local GCvalue = self:MakeMakeGameplayCue()
    self:K2_AddGameplayCueWithParams(self.GameplayCueTag, GCvalue)
end

function M:OnAdded()
    self:K2_CancelAbility()
end

function M: K2_OnEndAbility(bWasCancelled)
    self:BP_RemoveGameplayEffectFromOwnerWithHandle(self.ApplyRageHandle,-1)
end
function M:MakeMakeGameplayCue()
    local SkeletalMeshComp = self:GetOwningComponentFromActorInfo()
    local zeroVec = UE.FVector(0, 0, 0)
    local zeroTag = UE.FGameplayTag()                   -- 空 tag
    local zeroTagContainer = UE.FGameplayTagContainer() -- 空容器
    local context = UE.FGameplayEffectContextHandle()
    local returnvalue = UE.UAbilitySystemBlueprintLibrary.MakeGameplayCueParameters(
        0.0, -- NormalizedMagnitude
        0.0, -- RawMagnitude
        context,
        zeroTag,
        zeroTag,
        zeroTagContainer,
        zeroTagContainer,
        zeroVec,
        zeroVec,
        nil,
        nil,
        nil,
        nil,
        1, -- GameplayEffectLevel
        1, -- AbilityLevel
        SkeletalMeshComp,
        false
    )
    return returnvalue
end

return M