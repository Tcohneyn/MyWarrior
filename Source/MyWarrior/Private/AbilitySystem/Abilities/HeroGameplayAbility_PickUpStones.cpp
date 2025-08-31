// Tcohneyn All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_PickUpStones.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Items/PickUps/WarriorStoneBase.h"
#include "Components/UI/HeroUIComponent.h"

void UHeroGameplayAbility_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{	
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_PickUpStones::CollectStones()
{
    // 清空已收集的石头数组，准备重新检测
	CollectedStones.Empty();

	TArray<FHitResult> TraceHits;
    // 使用 BoxTraceMultiForObjects 进行方盒检测，寻找角色下方的石头
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + -GetHeroCharacterFromActorInfo()->GetActorUpVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		(-GetHeroCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		StoneTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits,
		true
	);
    // 遍历命中的所有物体
	for (const FHitResult& TraceHit : TraceHits)
	{
	    // 判断是否是 AWarriorStoneBase（可拾取石头）
		if (AWarriorStoneBase* FoundStone = Cast<AWarriorStoneBase>(TraceHit.GetActor()))
		{
		    // 添加到 CollectedStones 数组中（不重复）
			CollectedStones.AddUnique(FoundStone);
		}
	}
    // 如果没检测到任何石头 → 取消技能
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroGameplayAbility_PickUpStones::ConsumeStones()
{
    // 如果没有收集到石头 → 取消技能
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}
    // 遍历已收集的石头
	for (AWarriorStoneBase* CollectedStone : CollectedStones)
	{
		if (CollectedStone)
		{
		    // 调用石头的 Consume 方法，传入角色的 ASC 和技能等级
			CollectedStone->Consume(GetWarriorAbilitySystemComponentFromActorInfo(),GetAbilityLevel());
		}
	}
}
