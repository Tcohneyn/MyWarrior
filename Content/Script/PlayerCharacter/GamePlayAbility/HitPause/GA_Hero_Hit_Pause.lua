local Screen = require("Widgets.Screen")
--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
require("LuaPanda").start("127.0.0.1",8818);

---@type GA_Hero_Hit_Pause_C
local M = UnLua.Class()

function M:camerashake()
    -- 获取当前角色的控制器
    local HeroController = self:GetHeroControllerFromActorInfo()
    if HeroController then
        -- 获取当前角色的摄像机组件
         HeroController:ClientStartCameraShake(self.CameraShakeClass, 1.0)
    end
end
function M:ExecuteHitPause()
    local world = self:GetWorld()
    -- 启动协程处理延迟逻辑
--[[     coroutine.wrap(function()
        -- 记录当前时间膨胀状态
        UE.UGameplayStatics.SetGlobalTimeDilation(world, 0.1)
        Screen.Print("时间膨胀已激活，当前膨胀系数: 0.1")

        -- 关键！使用Unreal原生延迟函数（Latent Function）
        UE.UKismetSystemLibrary.Delay(world, self.HitPauseDuration)
        
        -- 恢复时间并结束技能
        UE.UGameplayStatics.SetGlobalTimeDilation(world, 1.0)
        Screen.Print("时间膨胀已恢复，当前膨胀系数: 1.0")
        self:K2_EndAbility()
    end)() ]]
    -- 记录当前时间膨胀状态
    UE.UGameplayStatics.SetGlobalTimeDilation(world, 0.1)
    --Screen.Print("时间膨胀已激活，当前膨胀系数: 0.1")
    coroutine.resume(coroutine.create(function()

        -- 关键！使用Unreal原生延迟函数（Latent Function）
        UE.UKismetSystemLibrary.Delay(world, self.HitPauseDuration)

        -- 恢复时间并结束技能
        UE.UGameplayStatics.SetGlobalTimeDilation(world, 1.0)
        --Screen.Print("时间膨胀已恢复，当前膨胀系数: 1.0")
        -- 执行摄像机震动
        self:camerashake()
        -- 结束技能
        self:K2_EndAbility()
    end))
end

function M:K2_ActivateAbilityFromEvent(EventData)
    self:ExecuteHitPause()
end

return M
