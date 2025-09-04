--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_SurvialGameMode_C
local M = UnLua.Class()


function M:ReceiveBeginPlay()
   self.OnSurvialGameModeStateChanged:Add(self, M.OnSurvialGameModeStateChanged_Event)
end

function M:OnSurvialGameModeStateChanged_Event(CurrentState)
   if (CurrentState == UE.EWarriorSurvialGameModeState.WaitSpawnNewWave) then
      local Text = "Wave " .. self.CurrentWaveCount .. " Coming in "
      --UE.UKismetSystemLibrary.PrintText(self,Text)
      self:CreateWaveWidgetWithCountDown(Text, self.SpawnNewWaveWaitTime)
   end
   if (CurrentState == UE.EWarriorSurvialGameModeState.SpawningNewWave) then
      local Text = "Wave " .. self.CurrentWaveCount
      --UE.UKismetSystemLibrary.PrintText(self, Text)
      self:CreateWaveWidgetNoCountDown(Text)
      if (self.CurrentWaveCount == self.TotalWavesToSpawn) then
         self.SpawnEnemiesDelayTime = 4.0
         coroutine.resume(
            coroutine.create(function()
               UE.UKismetSystemLibrary.Delay(self, 2.0)
               local SecondText = "Final Wave"
               self:CreateWaveWidgetNoCountDown(SecondText)
            end))
      end
   end
   if (CurrentState == UE.EWarriorSurvialGameModeState.AllWavesCompleted) then
      local Text = "Wave " .. self.CurrentWaveCount .. " Completed "
      --UE.UKismetSystemLibrary.PrintText(self,Text)
      self:CreateWaveWidgetWithCountDown(Text, self.WaveCompletedWaitTime)
   end
   if (CurrentState == UE.EWarriorSurvialGameModeState.AllWavesDone) then
      UE.UKismetSystemLibrary.PrintText(self, "All Waves Done")
   end
      if (CurrentState == UE.EWarriorSurvialGameModeState.PlayDied) then
      UE.UKismetSystemLibrary.PrintText(self, "Hero Died")
   end
end

function M:CreateWaveWidgetWithCountDown(TextContenttoDisplay, InTotalCountDownTime)
   local Widget = UE.UWidgetBlueprintLibrary.Create(self, self.WaveCountDownWidgetClass)
   Widget:AddToViewport()
   Widget.TextBox_WaveText:SetText(TextContenttoDisplay)
   Widget:StartCountDown(InTotalCountDownTime)
end

function M:CreateWaveWidgetNoCountDown(TextContenttoDisplay)
   local Widget = UE.UWidgetBlueprintLibrary.Create(self, self.WaveWidgetClass)
   Widget:AddToViewport()
   Widget.TextBox_WaveText:SetText(TextContenttoDisplay)
end

return M
