--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type TPWBP_IconSlot_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

 function M:Construct()
    self:LoadSoftTextureAndSetAsIcon(self.SoftTexture)
end

function M:LoadSoftTextureAndSetAsIcon(SoftTexture)
    if SoftTexture == nil then
        self.Image_icon:SetVisibility(UE.ESlateVisibility.Hidden)
    else
        local AssetId = UE.UKismetSystemLibrary.GetPrimaryAssetIdFromSoftObjectReference(self,SoftTexture)
        local AsyncActionLoadPrimaryAsset = UE.UAsyncActionLoadPrimaryAsset.AsyncLoadPrimaryAsset(self,AssetId, nil)
        AsyncActionLoadPrimaryAsset.Completed:Add(self, self.ReceiveLoadedTexture)
        AsyncActionLoadPrimaryAsset:Activate()
    end

end

    function M:ReceiveLoadedTexture(Object)
        local Texture = UE.UTexture2D.Cast(Object)
        if Texture then
            self.Image_icon:SetBrushFromTexture(Texture)
            self.Image_icon:SetVisibility(UE.ESlateVisibility.Visible)
        end
    end

return M
