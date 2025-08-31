--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Hero_Overlay_C
local M = UnLua.Class()
local Screen = require "Widgets.Screen"

function M:Construct()
    self.WBP_ConsumeStoneKey:SetVisibility(UE.ESlateVisibility.Hidden)
end

function M:BP_OnOwningHeroUIComponentInitialized(OwningHeroUIComponent)
    -- This function is called when the owning Hero UI component is initialized.
    -- You can add your initialization logic here.
    self.HeroUIComponent = OwningHeroUIComponent
    if self.HeroUIComponent.OnCurrentHealthChanged then
        self.HeroUIComponent.OnCurrentHealthChanged:Add(self, M.OnCurrentHealthChanged)
    end
    if self.HeroUIComponent.OnCurrentRageChanged then
        self.HeroUIComponent.OnCurrentRageChanged:Add(self, M.OnCurrentRageChanged)
    end
    if self.HeroUIComponent.OnEquippedWeaponChanged then
        self.HeroUIComponent.OnEquippedWeaponChanged:Add(self, M.OnEquippedWeaponChanged)
    end
    if self.HeroUIComponent.OnStoneInteracted then
        self.HeroUIComponent.OnStoneInteracted:Add(self, M.OnStoneInteracted)
    end
end

function M:OnCurrentHealthChanged(NewPercent)
    local Owner = self.HeroUIComponent:GetOwner()

    -- 校验 Owner 有效性
    if not Owner or not Owner:IsValid() then
        return
    end

    -- 调用 Owner 的方法（使用冒号传递 self）
    --local b = UE.UKismetSystemLibrary.GetDisplayName(Owner)
    --local msg = [[Enemy UI Component valid Owner]] .. b .. [[Current Health Percent: ]] .. tostring(NewPercent)
    --Screen.Print(msg)
    if not self.WBP_HeroHealthBar then
        print("[ERROR] WBP_HeroHealthBar is nil!")
        return
    end
    self.WBP_HeroHealthBar:SetStatusBarPercent(NewPercent)
end

function M:OnCurrentRageChanged(NewPercent)
    local Owner = self.HeroUIComponent:GetOwner()

    -- 校验 Owner 有效性
    if not Owner or not Owner:IsValid() then
        return
    end
    self.WBP_HeroRageBar:SetStatusBarPercent(NewPercent)
end

function M:OnEquippedWeaponChanged(SoftWeaponIcon)
    self.WBP_HeroWeaponSlot:LoadSoftTextureAndSetAsIcon(SoftWeaponIcon)
end

function M:OnStoneInteracted(bShouldDisplayInputKey)
    if bShouldDisplayInputKey then
        self.WBP_ConsumeStoneKey:SetVisibility(UE.ESlateVisibility.Visible)
        local PlayerController = self:GetOwningPlayer()
        local Subsystem = UE.UWarriorFunctionLibrary.GetEnhancedInputLocalPlayerSubsystem(PlayerController)
        local Keys = Subsystem:QueryKeysMappedToAction(self.Action)
        local Keynum = Keys:Num()
        --UE.UKismetSystemLibrary.PrintString(self,"Keynum:" .. tostring(Keynum))
        if Keynum>0 then
        local Key = Keys[1]
        local Text = UE.UKismetInputLibrary.Key_GetDisplayName(Key, true)
        self.WBP_ConsumeStoneKey.WarriorTextBox_InputKeyText:SetText(Text)
        end
    else
        self.WBP_ConsumeStoneKey:SetVisibility(UE.ESlateVisibility.Hidden)
    end
end

return M
