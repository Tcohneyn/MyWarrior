// Tcohneyn All Rights Reserved


#include "WarriorTypes/WarriorCountDownAction.h"

void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
    // 1. 检查是否需要取消
    if (bNeedToCancel)
    {
        CountDownOutput = EWarriorCountDownActionOutput::Cancelled; // 标记状态为 Cancelled

        // 结束 Latent Action，并触发回调
        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

        return; // 退出，不再继续
    }

    // 2. 检查是否已经到达总时长（倒计时结束）
    if (ElapsedTimeSinceStart >= TotalCountDownTime)
    {
        CountDownOutput = EWarriorCountDownActionOutput::Completed; // 标记完成

        // 完成并触发回调
        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

        return;
    }

    // 3. 如果还没到结束，就检查更新间隔
    if (ElapsedInterval < UpdateInterval)
    {
        // 累加经过的时间（每帧）
        ElapsedInterval += Response.ElapsedTime();
    }
    else
    {
        // 时间已到达更新间隔：
        // 累加总时间（这里逻辑：如果 UpdateInterval > 0，就按间隔累加，否则直接用 delta time）
        ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

        // 更新剩余时间
        OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

        // 标记状态为 Updated（倒计时进行中）
        CountDownOutput = EWarriorCountDownActionOutput::Updated;

        // 触发蓝图连线（不结束整个 Action，只是通知一次）
        Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		  
        // 重置间隔时间
        ElapsedInterval = 0.f;
    }
}

void FWarriorCountDownAction::CancelAction()
{
    // 设置取消标记
    bNeedToCancel = true;
}

