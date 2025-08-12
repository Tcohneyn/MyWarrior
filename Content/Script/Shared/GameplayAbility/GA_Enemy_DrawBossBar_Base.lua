--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Enemy_DrawBossBar_Base_C
local M = UnLua.Class()

function M:K2_ActivateAbility()
    -- Optionally, you can call the parent class's method if needed
    local EnemyCharacter = self:GetEnemyCharacterFromActorInfo()
    local MyHUDClass = UE.UClass.Load("/Game/Widgets/EnemyWidgets/WBP_DefaultBossHealthBar.WBP_DefaultBossHealthBar_C")
    -- 规范对象创建（指定Outer和Owner）
    self.MyHUD = UE.UWidgetBlueprintLibrary.Create(self, MyHUDClass)
    self.MyHUD.TextBlock_BossNameText:SetText(self.BossName)
    self.MyHUD:AddToViewport()
    self.MyHUD:InitEnemyCreatedWidget(EnemyCharacter)
    EnemyCharacter.EnemyUIComponent:RegisterEnemyDrawnWidget(self.MyHUD)
    self:K2_EndAbility()
end 


return M