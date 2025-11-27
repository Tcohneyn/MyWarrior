// Tcohneyn All Rights Reserved


#include "GameModes/WarriorSurvialGameMode.h"

#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"

#include "WarriorDebugHelper.h"

void AWarriorSurvialGameMode::BeginPlay()
{
    Super::BeginPlay();
    // 确保生存模式的敌人波次数据表已经被赋值（否则崩溃）
    checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid datat table in survial game mode blueprint"));
    // 设置初始状态：等待生成新的敌人波次
    SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);
    // 统计总共有多少波敌人要生成（根据数据表行数）
    TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
    // 预加载下一波敌人的资源（异步加载）
    PreLoadNextWaveEnemies();
}

void AWarriorSurvialGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 状态：等待生成新的一波
    if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::WaitSpawnNewWave)
    {
        TimePassedSinceStart += DeltaTime;
        if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
        {

            TimePassedSinceStart = 0.f;
            // 切换到生成新波次状态
            SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::SpawningNewWave);
        }
    }
    // 状态：正在生成新波次
    if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::SpawningNewWave)
    {
        TimePassedSinceStart += DeltaTime;
        if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
        {
            // 生成敌人，计数器增加
            CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

            TimePassedSinceStart = 0.f;
            // 切换到“战斗进行中”状态
            SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::InProgress);
        }
    }
    // 状态：当前波完成，等待下一波
    if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::WaveCompleted)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= WaveCompletedWaitTime)
        {
            TimePassedSinceStart = 0.f;

            CurrentWaveCount++;
            // 如果所有波次都完成了，进入结束状态
            if (HasFinishedAllWaves())
            {
                SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::AllWavesDone);
            }
            else
            {
                // 否则，进入等待下一波状态，并预加载敌人
                SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);
                PreLoadNextWaveEnemies();
            }
        }
    }
}


void AWarriorSurvialGameMode::SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState InState)
{
    CurrentSurvialGameModeState = InState;
    // 广播事件，通知状态变化（可供蓝图或 UI 绑定）
    OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}

bool AWarriorSurvialGameMode::HasFinishedAllWaves() const
{
    // 判断是否打完所有波次
    return CurrentWaveCount > TotalWavesToSpawn;
}

void AWarriorSurvialGameMode::PreLoadNextWaveEnemies()
{
    if (HasFinishedAllWaves())
    {
        return;
    }

    PreLoadedEnemyClassMap.Empty();
    // 遍历下一波的敌人配置
    for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
        // 异步加载敌人类
        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateLambda(
                [SpawnerInfo,this]()
                {
                    if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
                    {
                        PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
                        Debug::Print(LoadedEnemyClass->GetName() + TEXT("is loaded"));
                    }
                }
                )
            );
    }
}

FWarrriorEnemyWaveSpawnerTableRow* AWarriorSurvialGameMode::GetCurrentWaveSpawnerTableRow() const
{
    // 根据当前波次号拼接出 RowName，例如 "Wave1"
    const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
    // 在数据表中查找对应的配置行
    FWarrriorEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarrriorEnemyWaveSpawnerTableRow>(RowName, FString());
    checkf(FoundRow, TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());
    return FoundRow;
}

int32 AWarriorSurvialGameMode::TrySpawnWaveEnemies()
{
    // 如果还没获取目标点，先从关卡中找
    if (TargetPointsArray.IsEmpty())
    {
        UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
    }
    checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level:%s for spawning enemies"), *GetWorld()->GetName());
    uint32 EnemiesSpawnedThisTime = 0;
    // 生成参数：如果位置冲突也强制生成
    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    // 遍历波次配置
    for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
        // 随机决定这次要生成多少只敌人（范围内随机）
        const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

        UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

        for (int32 i = 0; i < NumToSpawn; i++)
        {
            // 随机挑一个 TargetPoint 作为生成点
            const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
            const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
            const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
            // 在导航网格半径 400 内找随机点
            FVector RandomLocation;
            UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);
            // 把生成位置往上抬 150，避免陷进地面
            RandomLocation += FVector(0.f, 0.f, 150.f);
            // 生成敌人
            AWarriorEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(LoadedEnemyClass, RandomLocation,
                SpawnRotation, SpawnParam);

            if (SpawnedEnemy)
            {
                // 敌人死亡时触发回调
                SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

                EnemiesSpawnedThisTime++;
                TotalSpawnedEnemiesThisWaveCounter++;
            }
            // 如果达到本波敌人上限，就直接返回
            if (!ShouldKeepSpawnEnemies())
            {
                return EnemiesSpawnedThisTime;
            }
        }
    }

    return EnemiesSpawnedThisTime;
}

bool AWarriorSurvialGameMode::ShouldKeepSpawnEnemies()
{
    // 判断是否还需要继续生成敌人（当前已生成数 < 本波要求数）
    return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AWarriorSurvialGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
    CurrentSpawnedEnemiesCounter--;

    //Debug::Print(FString::Printf(TEXT("CurrentSpawnedEnemiesCounter:%i,TotalSpawnEnemiesThisWaveCounter:%i"), CurrentSpawnedEnemiesCounter,TotalSpawnedEnemiesThisWaveCounter));
    // 如果还有敌人要生成，就继续刷怪
    if (ShouldKeepSpawnEnemies())
    {
        CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
    }
    // 否则，如果场上已无敌人，说明波次结束
    else if (CurrentSpawnedEnemiesCounter == 0)
    {
        TotalSpawnedEnemiesThisWaveCounter = 0;
        CurrentSpawnedEnemiesCounter = 0;
        // 切换到“波次完成”状态
        SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaveCompleted);
    }
}

void AWarriorSurvialGameMode::RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRegister)
{
    // 外部传入一批敌人，手动注册到计数器里
    for (AWarriorEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
    {
        if (SpawnedEnemy)
        {
            CurrentSpawnedEnemiesCounter++;
            // 绑定敌人死亡事件
            SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
        }
    }
}