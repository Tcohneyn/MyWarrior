--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTService_UpdateMotionWarpAttackTarget_C
local M = UnLua.Class()

function M:ReceiveTickAI(_OwnerController,ControlledPawn,DeltaSeconds)
    if not (self.OwningEnemyCharacter and self.OwningEnemyCharacter:IsValid()) then
      UE.UKismetSystemLibrary.PrintString(self, "Error: OwningEnemyCharacter is not valid.")
              local EnemyChar = ControlledPawn:Cast(UE.AWarriorEnemyCharacter)
        if not EnemyChar then
            -- cast 失败，Finish Execute (失败)
            UE.UKismetSystemLibrary.PrintString(self, "Error: ControlledPawn is not AWarriorEnemyCharacter.")
            return 
        end
        self.OwningEnemyCharacter = EnemyChar
    end
    local MotionWarpingComponent = self.OwningEnemyCharacter.MotionWarpingComponent
    if not MotionWarpingComponent then
        UE.UKismetSystemLibrary.PrintString(self, "Error: MotionWarpingComponent is not valid.")
        return
    end
    local actor = UE.UBTFunctionLibrary.GetBlackboardValueAsActor(self, self.Key)
    local TargetLocation = actor:K2_GetActorLocation()
    if not actor or not actor:IsValid() then
        UE.UKismetSystemLibrary.PrintString(self, "Error: Actor is not valid.")
        return
    end
    MotionWarpingComponent:AddOrUpdateWarpTargetFromLocation(self.WarpTargetName,TargetLocation)
end

return M