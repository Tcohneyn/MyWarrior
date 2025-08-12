--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Giant_SummonEnemies_C
local M = UnLua.Class("Shared.GameplayAbility.GA_Enemy_SummonEnemies_Base")

function M:K2_ActivateAbility()
    self.Super.K2_ActivateAbility(self)
end

function M:OnMontage()
    self.Super.OnMontage(self)
end

return M