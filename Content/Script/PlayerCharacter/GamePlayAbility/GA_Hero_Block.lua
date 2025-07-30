--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_Block_C
local M = UnLua.Class()
function M:K2_ActivateAbility()
    self.HeroBlockTime = UE.UGameplayStatics.GetTimeSeconds(self:GetWorld())
    local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
        self.MontagetoPlay)
    PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
    PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
    PlayMontageTask:ReadyForActivation()
    self:RunSequence()
end

function M:RunSequence(MyTarget, Parameters)
    -- 创建协程任务链
    -- 延迟0.1秒后执行 Task1

    self:Task1()
    -- 延迟0.2秒后执行 Task2

    self:Task2()
end

function M:Task1()
    local GCvalue = self:MakeMakeGameplayCue()
    self:K2_AddGameplayCueWithParams(self.GameplayCueTag, GCvalue)
end

function M:Task2()
    local WaitEventTask = UE.UAbilityTask_WaitGameplayEvent.WaitGameplayEvent(self, self.EventTag)
    WaitEventTask.EventReceived:Add(self, M.OnEventReceived)
    WaitEventTask:ReadyForActivation()
end

function M:OnEventReceived(Payload)
    local bPerfectBlock = UE.UGameplayStatics.GetTimeSeconds(self:GetWorld()) - self.HeroBlockTime<=0.25
    --格挡转向
    local character = self:GetHeroCharacterFromActorInfo()
    local Start = character:K2_GetActorLocation()
    local Target = Payload.Instigator:K2_GetActorLocation()
    local Rotation = UE.UKismetMathLibrary.FindLookAtRotation(Start, Target)
    character:K2_SetActorRotation(Rotation,false)

    --格挡击退
    local forwardvector = character:GetActorForwardVector()* -1.0
    local Task = UE.UAbilityTask_ApplyRootMotionConstantForce.ApplyRootMotionConstantForce(
        self,                                                        -- OGS (Ability)
        "RollRootMotion",                                            -- 任务名字
        forwardvector,                                               -- 方向向量
        80.0,                                                      -- 强度
        0.2,                                                         -- 持续时间
        false,                                                       -- IsAdditive
        nil,                                                         -- StrengthOverTimeCurve（可以传 nil）
        UE.ERootMotionFinishVelocityMode.MaintainLastRootMotionVelocity, -- VelocityOnFinishMode
        UE.FVector(0, 0, 0),                                         -- SetVelocityOnFinish
        0.0,                                                         -- ClampVelocityOnFinish
        true                                                         -- EnableGravity
    )
    Task:ReadyForActivation()

    --格挡效果
     local GCvalue = self:MakeMakeGameplayCue()
    self:K2_ExecuteGameplayCueWithParams(self.GameplayCueTag1, GCvalue)

    --完美格挡
    if bPerfectBlock then
        UE.UWarriorFunctionLibrary.AddGameplayTagToActorIfNone(character, self.PerfectBlockTag)
        local GCvalue = self:MakeMakeGameplayCue()
        self:K2_ExecuteGameplayCueWithParams(self.GameplayCueTag2, GCvalue)
         local world = self:GetWorld()
            UE.UGameplayStatics.SetGlobalTimeDilation(world, 0.2)
    --Screen.Print("时间膨胀已激活，当前膨胀系数: 0.1")
    coroutine.resume(coroutine.create(function()

        -- 关键！使用Unreal原生延迟函数（Latent Function）
        UE.UKismetSystemLibrary.Delay(world, 0.08)

        -- 恢复时间并结束技能
        UE.UGameplayStatics.SetGlobalTimeDilation(world, 1.0)
        --Screen.Print("时间膨胀已恢复，当前膨胀系数: 1.0")
    end))
    end
end

function M:K2_OnEndAbility()
    local world = self:GetWorld()
    local value = UE.UGameplayStatics.GetGlobalTimeDilation(world)
    if value ~= 1.0 then
        UE.UGameplayStatics.SetGlobalTimeDilation(world, 1.0)
        --Screen.Print("时间膨胀已恢复，当前膨胀系数: 1.0")
    end
end
function M:OnMontage()
    -- Montage 播放结束后结束 Ability（对应蓝图连接 EndAbility）
    self:K2_EndAbility()
    self:StartResetJumpToFinisherTimer()
end

function M:StartResetJumpToFinisherTimer()
    UE.UKismetSystemLibrary.K2_SetTimerDelegate({self,self.ResetJumpToFinisherState},0.3)
end

function M:ResetJumpToFinisherState()
     UE.UWarriorFunctionLibrary.RemoveGameplayTagFromActor(self:GetHeroCharacterFromActorInfo(), self.PerfectBlockTag)
     return 
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
