// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/WarriorPickUpBase.h"
#include "WarriorStoneBase.generated.h"

class UGameplayEffect;
class UWarriorAbilitySystemComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FLuaOnAssetLoaded,TSubclassOf<UObject>, Loaded);

UCLASS()
class MYWARRIOR_API AWarriorStoneBase : public AWarriorPickUpBase
{
	GENERATED_BODY()
public:
    UPROPERTY()
    FLuaOnAssetLoaded OnStoneAssetLoaded;
    
    void Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent,int32 ApplyLevel);
protected:
    virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
    void BP_OnStoneConsumed();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;


};
