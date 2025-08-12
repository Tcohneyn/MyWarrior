--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Giant_MeleeAttack2_C
local M = UnLua.Class("Shared.GameplayAbility.GA_Warrior_Enemy_MeleeAttack_Base")
function M:K2_ActivateAbility()
    self.Super.K2_ActivateAbility(self)
end
return M