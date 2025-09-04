--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_Death_C
local M = UnLua.Class()
function M:K2_ActivateAbility()
    local length = self.MontagetoPlay:Num()
    local choice = math.random(1, length)
    local MontagetoPlays = self.MontagetoPlay[choice]
    local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        MontagetoPlays)
    PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
    PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
end

function M:K2_OnEndAbility(bWasCancelled)
    self:GetOwningComponentFromActorInfo().bPauseAnims = true
    local characterCap = self:GetHeroCharacterFromActorInfo().CapsuleComponent
    if characterCap then
        characterCap:SetCollisionEnabled(UE.ECollisionEnabled.NoCollision)
    end
    local controller = self:GetHeroControllerFromActorInfo()
    local GameMode = UE.UGameplayStatics.GetGameMode(self)
    local SGameMode = GameMode:Cast(UE.AWarriorSurvialGameMode)
    SGameMode:SetCurrentSurvialGameModeState(UE.EWarriorSurvialGameModeState.PlayDied)
    if controller and controller:IsValid() then
        UE.UWidgetBlueprintLibrary.SetInputMode_UIOnlyEx(controller)
        controller.bShowMouseCursor = true
    end
end

function M:OnMontage()
   self:K2_EndAbility()
end
return M