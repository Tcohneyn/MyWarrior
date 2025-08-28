// Tcohneyn All Rights Reserved

#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WarriorDebugHelper.h"
#include "WarriorGameplayTags.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "WarriorTypes/WarriorCountDownAction.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
    if (!IsValid(InActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("NativeGetWarriorASCFromActor: InActor is null or invalid"));
        return nullptr;
    }
    check(InActor);
    //if (!IsValid(InActor))
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("NativeGetWarriorASCFromActor: InActor is null or invalid"));
    //    return nullptr;
    //}
    return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    // 从传入的 Actor 上获取自定义的 AbilitySystemComponent
    UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        // 就给它添加一个 Loose GameplayTag
        ASC->AddLooseGameplayTag(TagToAdd);
        //Loose GameplayTag 不依赖于 GameplayEffect，它只是单纯存在于 ASC 上，可以用来当作状态标记或条件检查
    }
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActor(AActor* InActor, FGameplayTag TagToRemove)
{
    UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {   // 就移除掉它（仅 Loose Tag）
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
    // Debug::Print(ASC->HasMatchingGameplayTag(TagToCheck) ? TEXT("true") : TEXT("false"));
    if (!ASC)
    {
        return false;
    }
    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
    check(InActor);
    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }
    return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
    OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::InValid;
    return CombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }
    return false;
}

float UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}

FHitReactResult UWarriorFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim)
{
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    FHitReactResult HitReactResult;
    HitReactResult.Angle = UKismetMathLibrary::DegAcos(DotResult);
    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);
    if (CrossResult.Z < 0.f)
    {
        HitReactResult.Angle *= -1.f;
    }
    if (HitReactResult.Angle >= -45.f && HitReactResult.Angle <= 45.f)
    {
        HitReactResult.Tag = WarriorGameplayTags::Shared_Status_HitReact_Front;
        return HitReactResult;
    }
    else if (HitReactResult.Angle < -45.f && HitReactResult.Angle >= -135.f)
    {
        HitReactResult.Tag = WarriorGameplayTags::Shared_Status_HitReact_Left;
        return HitReactResult;
    }
    else if (HitReactResult.Angle < -135.f || HitReactResult.Angle > 135.f)
    {
        HitReactResult.Tag = WarriorGameplayTags::Shared_Status_HitReact_Back;
        return HitReactResult;
    }
    else if (HitReactResult.Angle > 45.f && HitReactResult.Angle <= 135.f)
    {
        HitReactResult.Tag = WarriorGameplayTags::Shared_Status_HitReact_Right;
        return HitReactResult;
    }

    HitReactResult.Tag = WarriorGameplayTags::Shared_Status_HitReact_Front;
    return HitReactResult;
}

bool UWarriorFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDenfender)
{
    check(InAttacker && InDenfender);
    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDenfender->GetActorForwardVector());

    //const FString DebugString =
    //    FString::Printf(TEXT("DotResult: %f %s"), DotResult, DotResult < -0.1f ? TEXT("Valid Block") : TEXT("Invalid Block"));

    //Debug::Print(DebugString,DotResult<-0.1f? FColor::Green : FColor::Red);

    return DotResult <-0.1f;
}

bool UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(
    AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UWarriorAbilitySystemComponent* SourceASC = NativeGetWarriorASCFromActor(InInstigator);
    UWarriorAbilitySystemComponent* TargetASC = NativeGetWarriorASCFromActor(InTargetActor);

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UWarriorFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime,
    EWarriorCountDownActionInput CountDownInput, EWarriorCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
    UWorld* World = nullptr;

    if (GEngine)
    {
        // 通过传入的 WorldContextObject 获取对应的世界（比如 Actor、GameInstance 等都能传进来）
        // 如果传的对象无效，这里可能返回 null
        World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
    }

    if (!World)
    {
        // 没拿到 World，就直接返回，不做后续逻辑
        return;
    }//这一步保证 LatentAction 必须运行在一个有效的 World 里，否则直接退出。

    // 获取当前世界的 LatentActionManager
    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

    //// 查找是否已经存在相同 UUID 的倒计时 Action
    //   LatentInfo.CallbackTarget → 蓝图节点的 UObject 拥有者
    //   LatentInfo.UUID           → 蓝图 Latent 节点的唯一 ID
    FWarriorCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FWarriorCountDownAction>(LatentInfo.CallbackTarget,LatentInfo.UUID);
    //Latent 节点的机制：同一个 CallbackTarget + UUID 组合下，只会存在一个对应的 Action。
    //所以这里先看看是不是已经有了一个正在跑的倒计时。

    if (CountDownInput == EWarriorCountDownActionInput::Start)
    {
        if (!FoundAction)
        {
            // 如果没有同类 Action，就新建一个
            LatentActionManager.AddNewAction(
                LatentInfo.CallbackTarget,
                LatentInfo.UUID,
                new FWarriorCountDownAction(
                    TotalTime,          // 倒计时总时长
                    UpdateInterval,     // 更新间隔
                    OutRemainingTime,// 引用：输出剩余时间
                    CountDownOutput, // 引用：输出状态枚举
                    LatentInfo)         // 蓝图节点上下文信息
            );
        }
    }

    if (CountDownInput == EWarriorCountDownActionInput::Cancel)
    {
        if (FoundAction)
        {
            // 找到已有 Action，就调用它的 CancelAction()
            FoundAction->CancelAction();
        }
    }
    
}

void UWarriorFunctionLibrary::AsyncLoadAsset(const UObject* WorldContextObject, TSoftObjectPtr<UObject> Asset, FLuaOnAssetLoaded OnLoaded)
{
    if (!Asset.IsValid() && Asset.ToSoftObjectPath().IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("AsyncLoadAsset: Asset is invalid"));
        return;
    }

    if (!OnLoaded.IsBound())
    {
        UE_LOG(LogTemp, Warning, TEXT("AsyncLoadAsset: OnLoaded delegate is not bound"));
        return;
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    const FSoftObjectPath SoftPath = Asset.ToSoftObjectPath();

    Streamable.RequestAsyncLoad(SoftPath, FStreamableDelegate::CreateLambda([SoftPath, OnLoaded]()
    {
        UObject* LoadedObject = SoftPath.ResolveObject();
        OnLoaded.ExecuteIfBound(LoadedObject);
    }));
}
