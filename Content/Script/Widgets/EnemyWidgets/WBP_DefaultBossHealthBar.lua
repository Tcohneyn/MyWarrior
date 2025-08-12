--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_DefaultBossHealthBar_C
local M = UnLua.Class()

function M:BP_OnOwningEnemyUIComponentInitialized(OwningEnemyUIComponent)
    -- This function is called when the owning Hero UI component is initialized.
    -- You can add your initialization logic here.
    self.EnemyUIComponent = OwningEnemyUIComponent
    if self.EnemyUIComponent.OnCurrentHealthChanged then
        self.EnemyUIComponent.OnCurrentHealthChanged:Add(self, self.OnCurrentHealthChanged)
    end
end

function M:OnCurrentHealthChanged(NewPercent)
       self.WBP_BossBar:SetStatusBarPercent(NewPercent)
    end
return M
