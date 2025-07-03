--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_DefaultEnemyHealthBar_C
local  WBP_DefaultEnemyHealthBar = UnLua.Class()

function WBP_DefaultEnemyHealthBar:BP_OnOwningEnemyUIComponentInitialized(OwningEnemyUIComponent)
    -- This function is called when the owning Hero UI component is initialized.
    -- You can add your initialization logic here.
    self.EnemyUIComponent = OwningEnemyUIComponent
    if self.EnemyUIComponent.OnCurrentHealthChanged then
        self.EnemyUIComponent.OnCurrentHealthChanged:Add(self, self.OnCurrentHealthChanged)
    end
end
function WBP_DefaultEnemyHealthBar:OnCurrentHealthChanged(NewPercent)
    local Owner = self.EnemyUIComponent:GetOwner()

    -- 校验 Owner 有效性
    if not Owner or not Owner:IsValid() then
        return
    end

    -- 调用 Owner 的方法（使用冒号传递 self）
    --local b = UE.UKismetSystemLibrary.GetDisplayName(Owner)
    --local msg = [[Enemy UI Component valid Owner]] .. b .. [[Current Health Percent: ]] .. tostring(NewPercent)
    --Screen.Print(msg)
    if not self.WBP_EnemyHealthBar then
        print("[ERROR] WBP_HeroHealthBar is nil!")
        return
    end
    UE.UKismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(self,self.TimerHandle)
    self.WBP_EnemyHealthBar:SetStatusBarPercent(NewPercent)

    if NewPercent == 0 or self.IsFirstBroadcast then
        self:SetVisibility(UE.ESlateVisibility.Hidden)
    else
        self:SetVisibility(UE.ESlateVisibility.Visible)
    end
   self.TimerHandle = UE.UKismetSystemLibrary.K2_SetTimerDelegate({self,self.callback},5.0,false)
    if self.IsFirstBroadcast then
        self.IsFirstBroadcast = false
    end
end


function WBP_DefaultEnemyHealthBar:callback()
    -- 这里可以添加定时器触发后的逻辑
    -- 比如重置状态或更新UI等
    print("Timer callback executed")
    self:SetVisibility(UE.ESlateVisibility.Hidden)
end

return WBP_DefaultEnemyHealthBar
