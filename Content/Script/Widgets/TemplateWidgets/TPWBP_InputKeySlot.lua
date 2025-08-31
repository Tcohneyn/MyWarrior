--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type TPWBP_InputKeySlot_C
local M = UnLua.Class()

function M:PreConstruct(IsDesignTime)
   self.WarriorSizeBox_Base:SetWidthHeightOverride(self.SizeBoxWidthOverride, self.SizeBoxHeightOverride)
end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
