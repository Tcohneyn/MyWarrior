--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Test_C
local M = UnLua.Class()
local Screen = require "Widgets.Screen"
function M:BP_OnOwningEnemyUIComponentInitialized(OwningEnemyUIComponent)
    -- This function is called when the owning Hero UI component is initialized.
    -- You can add your initialization logic here.
    self.EnemyUIComponent = OwningEnemyUIComponent
    if self.EnemyUIComponent.OnCurrentHealthChanged then
        self.EnemyUIComponent.OnCurrentHealthChanged:Add(self,M.OnCurrentHealthChanged)
    end

end

function M:OnCurrentHealthChanged(NewPercent)

    local Owner = self.EnemyUIComponent:GetOwner()

    -- 校验 Owner 有效性
    if not Owner or not Owner:IsValid() then
        return
    end

    -- 调用 Owner 的方法（使用冒号传递 self）
    local b = UE.UKismetSystemLibrary.GetDisplayName(Owner)
    local msg = [[Enemy UI Component valid Owner]] .. b..[[Current Health Percent: ]] .. tostring(NewPercent)
    Screen.Print(msg)
    end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
