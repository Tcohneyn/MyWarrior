--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Hero_DrawOverlayWidget_C
local M = UnLua.Class()

function M:K2_ActivateAbility()
    -- Optionally, you can call the parent class's method if needed
    local HeroController = self:GetHeroControllerFromActorInfo()
    if HeroController then
        local MyHUDClass = UE.UClass.Load("/Game/Widgets/HeroWidgets/WBP_Hero_Overlay.WBP_Hero_Overlay_C")
        -- 规范对象创建（指定Outer和Owner）
        self.MyHUD = UE.UWidgetBlueprintLibrary.Create(self, MyHUDClass, HeroController)
        self.MyHUD:AddToViewport()
    end
    self:K2_EndAbility()
end 

return M
