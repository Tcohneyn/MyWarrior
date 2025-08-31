--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type GA_Enemy_SpawnStone_Base_C
local M = UnLua.Class()
function M:K2_ActivateAbility()
    self:RunSequence()
end

function M:RunSequence()
   self:Task1()
   self:Task2()
end

function M:Task1()
    local Enemy = self:GetEnemyCharacterfromActorInfo()
    local Start = Enemy:K2_GetActorLocation()
    local End = Start + Enemy:GetActorUpVector() * -1.0 * 200.0
    local HitResult = UE.FHitResult()
    local IgnoreActors = UE.TArray(UE.AActor)
    UE.UKismetSystemLibrary.LineTraceSingleForObjects(self, Start, End, self.ObjectTypes, false, IgnoreActors,
        self.DrawDebugType, HitResult, true)
    if HitResult.ImpactPoint then
        UE.UNavigationSystemV1.K2_GetRandomLocationInNavigableRadius(self, HitResult.ImpactPoint, self.RandomSpawnLocation, 200.0,
             nil, nil)
        --UE.UKismetSystemLibrary.PrintString(self, "RandomSpawnLocation is :" .. tostring(self.RandomSpawnLocation))
    end
end

function M:Task2()
    if UE.UKismetMathLibrary.RandomBoolWithWeight(0.5) then
       self.stoneclass = self.StoneClasstoSpawn
    else
        self.stoneclass = self.StoneClasstoSpawn0
    end
    local weight = UE.UWarriorFunctionLibrary.GetScalableFloatValueAtLevel(self.StoneSpawnChanceScalableFloat,1.0)
    if UE.UKismetMathLibrary.RandomBoolWithWeight(weight) then
        self:OnClassLoaded()
    else
       self:K2_EndAbility()
    end
end

function M:OnClassLoaded()
    if self.stoneclass then
        self:SpawnStone(self.stoneclass)
        self:K2_EndAbility()
    else
        self:K2_EndAbility()
    end
end

function M:SpawnStone(stone)
    local World = self:GetWorld()
     if not World then
        return
     end

    -- 定义单独的值
    local SpawnRotation = UE.FRotator(0.0,0.0,UE.UKismetMathLibrary.RandomFloatInRange(0.0,360.0))
    local SpawnScale    = UE.FVector(1.0, 1.0, 1.0)
    local Transform = UE.UKismetMathLibrary.MakeTransform(self.RandomSpawnLocation, SpawnRotation, SpawnScale)
        -- 调用 SpawnActor
    local NewActor = World:SpawnActor(
        stone,
        Transform,
        UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn
    )
end
return M