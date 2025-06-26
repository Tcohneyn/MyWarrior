// Tcohneyn All Rights Reserved


#include "AbilitySystem/GameplayCues/Hero_AxeHit_GCNotify_Base.h"
//#include "Hero_AxeHit_GCNotify_Base.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectTypes.h"

void UHero_AxeHit_GCNotify_Base::HandleGameplayCue(
    AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
        Super::HandleGameplayCue(MyTarget, EventType, Parameters);

}

bool UHero_AxeHit_GCNotify_Base::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
    FVector Location = Target->GetActorLocation();
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
    return Super::OnExecute_Implementation(Target, Parameters);
}
