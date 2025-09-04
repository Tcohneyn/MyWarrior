--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Enemy_SummonEnemies_Base_C
local M = UnLua.Class()

function M:K2_ActivateAbility()
    local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        self.MontagetoPlay)
    PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
    PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
    local Enemy = self:GetEnemyCharacterFromActorInfo()
    local vector = Enemy:K2_GetActorLocation()
    --local Rotator = UE.UKismetMathLibrary.Conv_VectorToRotator(Enemy:GetActorForwardVector())
    local WaitSpawnTask = UE.UAbilityTask_WaitSpawnEnemies.WaitSpawnEnemies(self, self.EventTag,self.SoftEnemyClasstoSpawn,self.NumtoSpawn,vector,self.RandomSpawnRadius)
    WaitSpawnTask.OnSpawnFinished:Add(self, M.SpawnFinished)
    WaitSpawnTask:ReadyForActivation()
end

function M:OnMontage()
    self:K2_EndAbility()
end

function M:SpawnFinished(SpawnedEnemies)
   local Enemy = self:GetEnemyCharacterFromActorInfo()
   local EnemyBlackboard = UE.UAIBlueprintHelperLibrary.GetBlackboard(Enemy)
   local Key =UE.UKismetSystemLibrary.MakeLiteralName("HasSpawnedEnemies")
   EnemyBlackboard:SetValueAsBool(Key, true)
   local GameMode= UE.UGameplayStatics.GetGameMode(self)
   local SGameMode = GameMode:Cast(UE.AWarriorSurvialGameMode)
   UE.AWarriorSurvialGameMode.RegisterSpawnedEnemies(SGameMode,SpawnedEnemies)
end

return M