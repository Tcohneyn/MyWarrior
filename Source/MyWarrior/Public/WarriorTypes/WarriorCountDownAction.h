// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "WarriorTypes/WarriorEnumTypes.h"

// 一个自定义的 Latent Action：倒计时
// Latent Action 是 UE 里蓝图里 "Delay"、"Async Load" 那类节点的底层机制。
// 它继承自 FPendingLatentAction，由 LatentActionManager 驱动，每帧调用 UpdateOperation。

class FWarriorCountDownAction : public FPendingLatentAction
{
public:
    // 构造函数
    // InTotalCountDownTime     → 倒计时总时长
    // InUpdateInterval         → 更新间隔（多久触发一次 "Updated" 输出）
    // InOutRemainingTime       → 输出的剩余时间（通过引用传递，蓝图或外部可以读到）
    // InCountDownOutput        → 当前倒计时的状态枚举（更新、完成、取消）
    // LatentInfo               → 蓝图 Latent 节点调用所需信息（执行函数、回调对象等）
    FWarriorCountDownAction(
        float InTotalCountDownTime,
        float InUpdateInterval,
        float& InOutRemainingTime,
        EWarriorCountDownActionOutput& InCountDownOutput,
        const FLatentActionInfo& LatentInfo)
    : bNeedToCancel(false)                         // 默认不取消
    , TotalCountDownTime(InTotalCountDownTime)     // 保存总时长
    , UpdateInterval(InUpdateInterval)             // 保存更新间隔
    , OutRemainingTime(InOutRemainingTime)         // 引用：剩余时间
    , CountDownOutput(InCountDownOutput)           // 引用：输出状态
    , ExecutionFunction(LatentInfo.ExecutionFunction) // 蓝图需要回调的函数名
    , OutputLink(LatentInfo.Linkage)               // 蓝图连线 index
    , CallbackTarget(LatentInfo.CallbackTarget)    // 蓝图对象（UObject）
    , ElapsedInterval(0.f)                         // 已经过的间隔时间
    , ElapsedTimeSinceStart(0.f)                   // 已经过的总时间
    {
    }

    // 每帧 LatentActionManager 会调用 UpdateOperation
    virtual void UpdateOperation(FLatentResponse& Response) override;

    // 提供一个外部取消倒计时的接口
    void CancelAction();

private:
    bool bNeedToCancel;     // 是否需要取消
    float TotalCountDownTime;   // 倒计时总时长
    float UpdateInterval;       // 更新间隔
    float& OutRemainingTime;    // 引用，保存剩余时间，供外部读取
    EWarriorCountDownActionOutput& CountDownOutput; // 引用，输出当前状态（更新/完成/取消）
    FName ExecutionFunction;    // 蓝图回调函数
    int32 OutputLink;           // 蓝图连线索引
    FWeakObjectPtr CallbackTarget; // 蓝图节点所在的对象
    float ElapsedInterval;      // 累计的间隔时间
    float ElapsedTimeSinceStart;// 累计的总时间
};
