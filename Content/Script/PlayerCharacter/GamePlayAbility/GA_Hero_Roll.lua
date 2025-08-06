--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_Roll_C
local M = UnLua.Class()
function M:K2_ActivateAbility()
    local world = self:GetWorld()
        -- 模仿蓝图中，先延迟0.05秒等待输入捕获（对应字幕里 delay 在启动 Compute Roll Direction 之前）
    coroutine.resume(coroutine.create(function()
        UE.UKismetSystemLibrary.Delay(world, self.Duration)
        self:ComputeRollDirectionAndDistance()
              -- 设置并播放翻滚的 Montage（Subtitle 中“Play Montage and Wait”节点）
        local PlayMontageTask = UE.UAbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(self, "PlayMontageTask",
            self.MontagetoPlay)
        PlayMontageTask.OnCompleted:Add(self, M.OnMontage)
        PlayMontageTask.OnBlendOut:Add(self, M.OnMontage)
        PlayMontageTask.OnInterrupted:Add(self, M.OnMontage)
        PlayMontageTask.OnCancelled:Add(self, M.OnMontage)
        PlayMontageTask:ReadyForActivation()
    end))
end

function M:OnMontage()
    -- Montage 播放结束后结束 Ability（对应蓝图连接 EndAbility）
    self:K2_EndAbility()
end

function M:ComputeRollDirectionAndDistance()
    local Character = self:GetHeroCharacterFromActorInfo()
    if not Character then
        UE.UKismetSystemLibrary.PrintString(self, "Character is invalid!")
        return
    end
    -- 获取最后一次有效移动输入向量，并归一化
    local Vector = Character:GetLastMovementInputVector()
    self.CachedRollingDirection = UE.UKismetMathLibrary.Normal(Vector)
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
            self:Task3()
        end)
        Delay2:ReadyForActivation()
    end)
    Delay1:ReadyForActivation()
end

-- 示例任务函数
function M:Task1()
    local Character = self:GetHeroCharacterFromActorInfo()
    local MotionWarpingComp = Character.MotionWarpingComponent
    local TargetRotation = UE.UKismetMathLibrary.MakeRotFromX(self.CachedRollingDirection)
    MotionWarpingComp:AddOrUpdateWarpTargetFromLocationAndRotation(self.WarpTargetName, self.TargetLocation,
        TargetRotation)
end

function M:Task2()
    local Character = self:GetHeroCharacterFromActorInfo()
    --UE.UKismetSystemLibrary.PrintString(self, "任务2执行")
    local vec = Character:K2_GetActorLocation()
    local levelfloat = UE.UWarriorFunctionLibrary.GetScalableFloatValueAtLevel(self.RollingDistanceScalableFloat,self:GetAbilityLevel())
    local startvec = vec + self.CachedRollingDirection * levelfloat
    local vec2 = Character:GetActorUpVector()
    local endvec = startvec + vec2 * -1.0 * 500.0
    local IgnoreActors = UE.TArray(UE.AActor)
    local HitResult = UE.FHitResult()
    UE.UKismetSystemLibrary.LineTraceSingleForObjects(self, startvec, endvec, self.ObjectTypes, false, IgnoreActors,
        self.DrawDebugType, HitResult, true)
    local MotionWarpingComp = Character.MotionWarpingComponent
    if HitResult.bBlockingHit then
         MotionWarpingComp:AddOrUpdateWarpTargetFromLocation(self.WarpTargetName1, HitResult.ImpactPoint)
    else
         --UE.UKismetSystemLibrary.PrintString(self, "No")
         MotionWarpingComp:RemoveWarpTarget(self.WarpTargetName1)
    end
end

function M:Task3()
   --[[  UE.UKismetSystemLibrary.PrintString(self, "任务3执行") ]]
end

return M
