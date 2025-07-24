--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTTask_EnemyBase_C
local M = UnLua.Class("EnemyCharacter.BTTask.BTTask_EnemyBase")

function M:ReceiveExecuteAI(OwnerController, ControlledPawn)
    self.Super.ReceiveExecuteAI(self, OwnerController, ControlledPawn)
end
function M:OnEnemyExecuteTask()
    --UE.UKismetSystemLibrary.PrintString(self, "ToggleStrafingState")
    if not self.OwningEnemyCharacter or not self.OwningEnemyCharacter:IsValid() then
        UE.UKismetSystemLibrary.PrintString(self, "Error: No ACharacter.")
        return -- 关键空指针防护
    end
local EnemyCharacterMovement = self.OwningEnemyCharacter.CharacterMovement
if not EnemyCharacterMovement then
    UE.UKismetSystemLibrary.PrintString(self, "Error: CharacterMovement is nil.")
    return
end
    if self.ShouldEnable then
        EnemyCharacterMovement.bOrientRotationToMovement = false
        if self.ShouldChangeMaxWalkSpeed then
            EnemyCharacterMovement.MaxWalkSpeed = self.StrafingWalkSpeed
            UE.UWarriorFunctionLibrary.AddGameplayTagToActorIfNone(self.OwningEnemyCharacter, self.StrafingTag)
        else
            UE.UWarriorFunctionLibrary.AddGameplayTagToActorIfNone(self.OwningEnemyCharacter, self.StrafingTag)
        end
    else
        EnemyCharacterMovement.bOrientRotationToMovement = true
        local BlackboardValue = UE.UBTFunctionLibrary.GetBlackboardValueAsFloat(self,
            self.InDefaultMaxWalkSpeedKey)
        if EnemyCharacterMovement.MaxWalkSpeed == BlackboardValue then
            EnemyCharacterMovement.MaxWalkSpeed = EnemyCharacterMovement.MaxWalkSpeed
        else
            EnemyCharacterMovement.MaxWalkSpeed = BlackboardValue
        end
        UE.UWarriorFunctionLibrary.RemoveGameplayTagFromActor(self.OwningEnemyCharacter, self.StrafingTag)
    end
end

return M
