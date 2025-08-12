--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTDecorator_CheckCurrentHealthPercent_C
local M = UnLua.Class()

-- 生命值状态计算函数
function M:CalculateCurrentHealthPercentage(ASC)
    if not ASC or not ASC:IsValid() then
        return -1.0  -- 返回无效值
    end

    -- 用 GAS 属性 Getter 取 FGameplayAttribute
    local healthAttr = UE.UWarriorAttributeSet.MakeCurrentHealthAttribute()
    local maxHealthAttr = UE.UWarriorAttributeSet.MakeMaxHealthAttribute()
    --print("healthAttr:", healthAttr)
    --print("maxHealthAttr:", maxHealthAttr)
    local HealthValue = ASC:GetGameplayAttributeValue(healthAttr)
    local MaxHealthValue = ASC:GetGameplayAttributeValue(maxHealthAttr)
    if not MaxHealthValue then
        UE.UKismetSystemLibrary.PrintString(self, "Warning: MaxHealthValue is nil, default to 0")
        MaxHealthValue = 0
    end

    if not HealthValue then
        UE.UKismetSystemLibrary.PrintString(self, "Warning: HealthValue is nil, default to 0")
        HealthValue = 0
    end
    -- 安全计算百分比
    return HealthValue / MaxHealthValue
    
end


function M:PerformConditionCheckAI(_OwnerController, ControlledPawn)
    if not UE.UKismetSystemLibrary.IsValid(ControlledPawn) then
        UE.UKismetSystemLibrary.PrintString(self, "错误：无效的控制对象")
        return false
    end

    local ASC = UE.UAbilitySystemBlueprintLibrary.GetAbilitySystemComponent(ControlledPawn)
    if not UE.UKismetSystemLibrary.IsValid(ASC) then
        UE.UKismetSystemLibrary.PrintString(self, "错误: 无效的AbilitySystemComponent")
        return false
    end

    local CurrentHealthPercent = self:CalculateCurrentHealthPercentage(ASC)
    self.CachedCurrentHealthPercent = CurrentHealthPercent

    if not self.CheckOperation then
        UE.UKismetSystemLibrary.PrintString(self, "错误: CheckOperation 未设置")
        return false
    end

    local bResult = false
    local op = self.CheckOperation
    local tol = 0.0001

    if op == UE.EWarriorDecoratorOperaty.IsLessThan then
        bResult = (CurrentHealthPercent < self.ThresholdToCheck)
    elseif op == UE.EWarriorDecoratorOperaty.IsLessThanOrEqualTo then
        bResult = (CurrentHealthPercent <= self.ThresholdToCheck)
    elseif op == UE.EWarriorDecoratorOperaty.IsEqualTo then
        bResult = math.abs(CurrentHealthPercent - self.ThresholdToCheck) <= tol
    elseif op == UE.EWarriorDecoratorOperaty.IsGreaterThanOrEqualTo then
        bResult = (CurrentHealthPercent >= self.ThresholdToCheck)
    elseif op == UE.EWarriorDecoratorOperaty.IsGreaterThan then
        bResult = (CurrentHealthPercent > self.ThresholdToCheck)
    end

    return bResult

end


return M