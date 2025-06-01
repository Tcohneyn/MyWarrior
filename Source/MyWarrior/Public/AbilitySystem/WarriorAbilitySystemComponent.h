// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WarriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYWARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	public:
    void OnAbilityInputPressed(const FGameplayTag& InputTag);
    void OnAbilityInputReleased(const FGameplayTag& InputTag);
};
