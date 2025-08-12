--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Giant_DrawBossBar_C
local M = UnLua.Class("Shared.GameplayAbility.GA_Enemy_DrawBossBar_Base")
function M:K2_ActivateAbility()
    self.Super.K2_ActivateAbility(self)
end
return M