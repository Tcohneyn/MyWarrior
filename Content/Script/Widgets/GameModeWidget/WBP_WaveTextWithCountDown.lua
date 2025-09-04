--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_WaveTextWithCountDown_C
local M = UnLua.Class()


function M:Construct()
    self:PlayAnimation(self.Wave_Entry_Animation)
end

function M:StartCountDown(InTotalCountDownTime)
    local OutRemainingTime = 0.0
    local CountDownActionOutput = UE.EWarriorCountDownActionOutput.Cancelled
    coroutine.resume(
        coroutine.create(function(WorldContectObject, TotalCountDownTime)
            UE.UWarriorFunctionLibrary.CountDown(WorldContectObject, TotalCountDownTime, 0.0, OutRemainingTime,
                UE.EWarriorCountDownActionInput.Start, CountDownActionOutput)
            UE.UKismetSystemLibrary.PrintString(WorldContectObject,
                "CountDown Finished" .. OutRemainingTime .. CountDownActionOutput)
        end), self, InTotalCountDownTime)
    if (CountDownActionOutput == UE.EWarriorCountDownActionOutput.Updated) then
        self.TextBox_CountDownText:SetText(OutRemainingTime)
    elseif (CountDownActionOutput == UE.EWarriorCountDownActionOutput.Completed) then
        self:RemoveFromParent()
    end
end

return M
