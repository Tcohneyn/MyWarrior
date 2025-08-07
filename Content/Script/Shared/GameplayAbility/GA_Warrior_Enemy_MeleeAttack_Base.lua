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
    local Enemy = self:GetEnemyCharacterFromActorInfo()
    local bHaveTag = UE.UWarriorFunctionLibrary.NativeDoesActorHaveTag(Enemy, self.TagtoCheck)
    if(bHaveTag) then
        if not self.GameplayCueTag then
            UE.UKismetSystemLibrary.PrintString(self, "[Error] Invalid GameplayCueTag!")
            return
        end
        self:K2_ExecuteGameplayCue(self.GameplayCueTag,nil)
        local world = self:GetWorld()
        coroutine.resume(coroutine.create(function()
        -- 关键！使用Unreal原生延迟函数（Latent Function）
        UE.UKismetSystemLibrary.Delay(world, 0.2)
    end))
    end
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
    self.Targetactor = InPayload.Target
    self.Payload = InPayload
    local GESpecHandle = self:BP_MakeEnemyDamageEffectSpecHandle(self.EffectClass, self.InDamageScalableFloat)
    if not GESpecHandle then
        UE.UKismetSystemLibrary.PrintString(self, "EffectSpec invalid!")
        return
    end
    self:BP_ApplyEffectSpecHandleToTarget(InPayload.Target, GESpecHandle)
    self:RunSequence()
end

-- 顺序执行任务（模拟Sequence节点）
function M:RunSequence()
    -- 创建协程任务链
    -- 延迟0.1秒后执行 Task1
    local Delay1 = UE.UAbilityTask_WaitDelay.WaitDelay(self, 0.1)
    Delay1.OnFinish:Add(self, function()
        self:Task1()
        -- 延迟0.2秒后执行 Task2
        local Delay2 = UE.UAbilityTask_WaitDelay.WaitDelay(self, 0.1)
        Delay2.OnFinish:Add(self, function()
            self:Task2()
        end)
        Delay2:ReadyForActivation()
    end)
    Delay1:ReadyForActivation()
end

function M:Task1()
    --UE.UKismetSystemLibrary.PrintString(self, "任务1执行")
    local Context = UE.FGameplayEffectContextHandle()
    self:K2_ExecuteGameplayCue(self.WeaponHitSoundGameplayCueTag,Context)
end

function M:Task2()
    --UE.UKismetSystemLibrary.PrintString(self, "任务2执行")
    UE.UAbilitySystemBlueprintLibrary.SendGameplayEventToActor(self.Targetactor, self.EventTag1,self.Payload)
        
end
return M
