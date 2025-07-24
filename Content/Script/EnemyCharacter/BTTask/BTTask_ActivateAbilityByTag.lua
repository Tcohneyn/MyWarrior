--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTTask_ActivateAbilityByTag_C
local M = UnLua.Class("EnemyCharacter.BTTask.BTTask_EnemyBase")
function M:ReceiveExecuteAI(OwnerController, ControlledPawn)
    self.Super.ReceiveExecuteAI(self, OwnerController, ControlledPawn)
end

function M:OnEnemyExecuteTask()
    if not self.OwningEnemyCharacter or not self.OwningEnemyCharacter:IsValid() then
        UE.UKismetSystemLibrary.PrintString(self, "Error: No ACharacter.")
        return -- 关键空指针防护
    end
local EnemyAbilitySystemComponent = self.OwningEnemyCharacter.WarriorAbilitySystemComponent
if not EnemyAbilitySystemComponent then
    UE.UKismetSystemLibrary.PrintString(self, "Error: EnemyAbilitySystemComponent is nil.")
    return
end
EnemyAbilitySystemComponent:TryActivateAbilityByTag(self.AbilityTagtoActivate)
end
return M