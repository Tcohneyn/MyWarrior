--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type TPWBP_AbilityIconSlot_C
local M = UnLua.Class("Widgets.TemplateWidgets.TPWBP_IconSlot")

-- function M:PreConstruct()
--     self.Super.PreConstruct(self)
--     UE.UKismetSystemLibrary.PrintString(self, "TPWBP_AbilityIconSlot:PreConstruct")
-- end

function M:Construct() 
   self.Super.Construct(self)
   self.WarriorTextBox_CooldownText:SetVisibility(UE.ESlateVisibility.Hidden)
end

function M:BP_OnOwningHeroUIComponentInitialized(OwningHeroUIComponent)
    -- This function is called when the owning Hero UI component is initialized.
    -- You can add your initialization logic here.
    self.HeroUIComponent = OwningHeroUIComponent
    if self.HeroUIComponent.OnAbilityIconSlotUpdated then
        self.HeroUIComponent.OnAbilityIconSlotUpdated:Add(self, M.OnAbilityIconSlotUpdated)
    end
end

function M:OnAbilityIconSlotUpdated(AbilityInputTag,SoftAbilityIconMaterial)
if AbilityInputTag == self.InAbilityInputTag then
    UE.UWarriorFunctionLibrary.AsyncLoadAsset(self,SoftAbilityIconMaterial,function(Object)
      local MI = Object:Cast(UE.UMaterialInterface)
      local returnvalue = UE.UKismetMaterialLibrary.CreateDynamicMaterialInstance(self,MI,nil)
      self.CreateDynamicIconMaterialInstance = returnvalue
      self.Image_icon:SetBrushFromMaterial(self.CreateDynamicIconMaterialInstance)
      self.Image_icon:SetVisibility(UE.ESlateVisibility.Visible)
end)
    --self.HeroUIComponent.OnChanged:Bind(self,M.OnLuaChanged)
end
end


return M
